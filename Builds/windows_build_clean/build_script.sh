#!/bin/bash
# =============================================================
# build_windows.sh
# Dieses Skript läuft in MSYS2 (MINGW64-Shell) auf Windows
# oder in einer Linux-Umgebung mit mingw-w64 Cross-Compiler
# =============================================================
set -e
APP_NAME="2048"
SRC="main.c"
OUT_DIR="dist"
BIN_DIR="$OUT_DIR/bin"
echo "=== 2048 Windows Build Script ==="
# --- Abhängigkeiten installieren (MSYS2) ---
if command -v pacman &>/dev/null; then
echo "[1/4] Installiere Abhängigkeiten via pacman..."
pacman -S --needed --noconfirm \
mingw-w64-x86_64-gtk4 \
mingw-w64-x86_64-gstreamer \
mingw-w64-x86_64-gst-plugins-base \
mingw-w64-x86_64-gst-plugins-good \
mingw-w64-x86_64-gst-plugins-bad \
mingw-w64-x86_64-gst-plugins-ugly \
mingw-w64-x86_64-libpng \
mingw-w64-x86_64-cairo \
mingw-w64-x86_64-gcc \
mingw-w64-x86_64-pkg-config
fi
# --- Kompilieren ---
echo "[2/4] Kompiliere $SRC..."
CFLAGS=$(pkg-config --cflags gtk4 gstreamer-1.0 libpng cairo)
LIBS=$(pkg-config --libs gtk4 gstreamer-1.0 libpng cairo)
gcc $SRC \
$CFLAGS \
$LIBS \
-lm \
-mwindows \
-o ${APP_NAME}.exe
echo "✓ ${APP_NAME}.exe erstellt"
# --- DLLs bündeln ---
echo "[3/4] Sammle DLLs..."
mkdir -p "$BIN_DIR"
cp ${APP_NAME}.exe "$BIN_DIR/"
# Kopiere alle benötigten DLLs aus MSYS2
copy_deps() {
local binary="$1"
ldd "$binary" | grep -i "mingw\|msys" | awk '{print $3}' | while read dll; do
if [ -f "$dll" ] && [ ! -f "$BIN_DIR/$(basename $dll)" ]; then
cp "$dll" "$BIN_DIR/"
echo " → $(basename $dll)"
copy_deps "$dll"
fi
done
}
copy_deps "$BIN_DIR/${APP_NAME}.exe"
# GStreamer Plugins kopieren
GST_PLUGIN_PATH=$(pkg-config --variable=pluginsdir gstreamer-1.0)
mkdir -p "$BIN_DIR/lib/gstreamer-1.0"
cp "$GST_PLUGIN_PATH"/libgstplayback.dll \
"$GST_PLUGIN_PATH"/libgstaudioconvert.dll \
"$GST_PLUGIN_PATH"/libgstaudioresample.dll \
"$GST_PLUGIN_PATH"/libgstvolume.dll \
"$GST_PLUGIN_PATH"/libgstwasapi.dll \
"$GST_PLUGIN_PATH"/libgstdirectsound.dll \
"$GST_PLUGIN_PATH"/libgstmpg123.dll \
"$GST_PLUGIN_PATH"/libgstid3demux.dll \
"$GST_PLUGIN_PATH"/libgsttypefindfunctions.dll \
"$GST_PLUGIN_PATH"/libgstcoreelements.dll \
"$BIN_DIR/lib/gstreamer-1.0/" 2>/dev/null || true
# GSettings schemas
mkdir -p "$BIN_DIR/share/glib-2.0/schemas"
cp /mingw64/share/glib-2.0/schemas/gschemas.compiled "$BIN_DIR/share/glib-2.0/schemas/"
# GTK Icons + Theme
mkdir -p "$BIN_DIR/share/icons"
cp -r /mingw64/share/icons/Adwaita "$BIN_DIR/share/icons/" 2>/dev/null || true
cp -r /mingw64/share/icons/hicolor "$BIN_DIR/share/icons/" 2>/dev/null || true
# --- App-eigene Assets ---
echo "[3b/4] Kopiere App-Assets..."
mkdir -p "$BIN_DIR/share/sounds"
mkdir -p "$BIN_DIR/share/icons/lucide-icons"
# Sounds (müssen im selben Verzeichnis liegen wie das Skript)
for f in pop.mp3 bonk.mp3 lofi-background.mp3; do
[ -f "$f" ] && cp "$f" "$BIN_DIR/share/sounds/" || echo " WARNUNG: $f nicht gefunden"
done
# Lucide Icons
if [ -d "lucide-icons" ]; then
cp lucide-icons/*.svg "$BIN_DIR/share/icons/lucide-icons/"
fi
echo "[4/4] Fertig!"
echo ""
echo "Dein Spiel liegt in: $BIN_DIR/"
echo "Zum Testen: cd $BIN_DIR && ./${APP_NAME}.exe"
echo ""
echo "Für einen Installer: Inno Setup mit '2048.iss' ausführen"