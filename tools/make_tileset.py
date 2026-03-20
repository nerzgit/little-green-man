#!/usr/bin/env python3
"""
make_tileset.py - 画像をリサイズしてタイルグリッドで切り出し、タイルセット画像を生成するツール

Usage:
    # ファイル指定
    python make_tileset.py map_1.png --tile-w 32 --tile-h 32 --cols 3 -o map_1_tileset.png

    # ディレクトリ指定 (配下の全画像を一括処理)
    python make_tileset.py ./materials --tile-w 32 --tile-h 32 --cols 3 --outdir ./output

Options:
    --tile-w      タイル幅 (px)                              [default: 32]
    --tile-h      タイル高さ (px)                            [default: 32]
    --cols        横列数                                      [default: 4]
    --output, -o  出力ファイルパス (ファイル指定時のみ有効)
    --outdir      出力ディレクトリ (ディレクトリ指定時)       [default: 入力ディレクトリと同じ]
"""

import argparse
import math
import sys
from pathlib import Path

try:
    from PIL import Image
except ImportError:
    print("Pillow が必要です: pip install Pillow", file=sys.stderr)
    sys.exit(1)


def make_tileset(
    input_path: str,
    tile_w: int,
    tile_h: int,
    cols: int,
    output_path: str,
) -> None:
    src = Image.open(input_path).convert("RGBA")
    src_w, src_h = src.size

    target_w = cols * tile_w
    scaled_h = src_h * target_w / src_w
    target_h = max(tile_h, math.ceil(scaled_h / tile_h) * tile_h)

    resized = src.resize((target_w, target_h), Image.NEAREST)

    tiles = []
    y = 0
    while y + tile_h <= target_h:
        x = 0
        while x + tile_w <= target_w:
            tiles.append(resized.crop((x, y, x + tile_w, y + tile_h)))
            x += tile_w
        y += tile_h

    num_tiles = len(tiles)
    num_rows = (num_tiles + cols - 1) // cols

    out = Image.new("RGBA", (cols * tile_w, num_rows * tile_h), (0, 0, 0, 0))
    for i, tile in enumerate(tiles):
        col = i % cols
        row = i // cols
        out.paste(tile, (col * tile_w, row * tile_h))

    out.save(output_path)
    print(f"✓ {num_tiles} タイル -> {output_path}  ({cols}列 x {num_rows}行, {tile_w}x{tile_h}px)")
    print(f"  リサイズ: {src_w}x{src_h} -> {target_w}x{target_h}")


IMAGE_EXTENSIONS = {".png", ".jpg", ".jpeg", ".bmp", ".gif", ".webp"}


def collect_images(directory: Path) -> list[Path]:
    return sorted(p for p in directory.rglob("*") if p.suffix.lower() in IMAGE_EXTENSIONS)


def main() -> None:
    parser = argparse.ArgumentParser(
        description="画像をリサイズしてタイルグリッドで切り出し、タイルセット画像を生成します"
    )
    parser.add_argument("input", help="入力画像ファイル or ディレクトリ")
    parser.add_argument("--tile-w", type=int, default=32, help="タイル幅 px (default: 32)")
    parser.add_argument("--tile-h", type=int, default=32, help="タイル高さ (default: 32)")
    parser.add_argument("--cols", type=int, default=4, help="横列数 (default: 4)")
    parser.add_argument("--output", "-o", default=None, help="出力ファイルパス (ファイル指定時のみ)")
    parser.add_argument("--outdir", default=None, help="出力ディレクトリ (ディレクトリ指定時, default: 入力ディレクトリ)")

    args = parser.parse_args()

    input_path = Path(args.input)
    if not input_path.exists():
        print(f"Error: 見つかりません: {input_path}", file=sys.stderr)
        sys.exit(1)

    if input_path.is_dir():
        images = collect_images(input_path)
        if not images:
            print(f"Error: 画像ファイルが見つかりません: {input_path}", file=sys.stderr)
            sys.exit(1)
        outdir = Path(args.outdir) if args.outdir else input_path
        outdir.mkdir(parents=True, exist_ok=True)
        print(f"{len(images)} ファイルを処理します...")
        for img_path in images:
            out = outdir / img_path.with_stem(img_path.stem + "_tileset").name
            make_tileset(str(img_path), args.tile_w, args.tile_h, args.cols, str(out))
    else:
        output_path = args.output or str(input_path.with_stem(input_path.stem + "_tileset"))
        make_tileset(str(input_path), args.tile_w, args.tile_h, args.cols, output_path)


if __name__ == "__main__":
    main()
