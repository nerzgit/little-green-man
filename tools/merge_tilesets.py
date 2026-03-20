#!/usr/bin/env python3
"""
merge_tilesets.py - フォルダ内の全画像を1枚のタイルセットに結合するツール

横幅は tile_w * max_cols px を上限とし、超えたら次の行へ折り返す。
行ごとの最大高さを保持し、次の行はその高さからスタートする。

Usage:
    python merge_tilesets.py <input_dir> [options]

Options:
    --tile-w      タイル幅 (最大幅の基準 px) [default: 32]
    --max-cols    1行の最大列数              [default: 10]
    --output, -o  出力ファイルパス           [default: <input_dir>/merged_tileset.png]

Example:
    python merge_tilesets.py ./materials --max-cols 10 -o ./output/merged.png
"""

import argparse
import sys
from pathlib import Path

try:
    from PIL import Image
except ImportError:
    print("Pillow が必要です: pip install Pillow", file=sys.stderr)
    sys.exit(1)

IMAGE_EXTENSIONS = {".png", ".jpg", ".jpeg", ".bmp", ".gif", ".webp"}


def collect_images(directory: Path) -> list[Path]:
    return sorted(p for p in directory.rglob("*") if p.suffix.lower() in IMAGE_EXTENSIONS)


def merge_tilesets(
    input_dir: str,
    output_path: str,
    max_cols: int = 10,
    tile_w: int = 32,
) -> None:
    images = collect_images(Path(input_dir))
    if not images:
        print(f"Error: 画像ファイルが見つかりません: {input_dir}", file=sys.stderr)
        sys.exit(1)

    max_width = max_cols * tile_w

    opened: list[Image.Image] = [
        Image.open(p).convert("RGBA") for p in images
    ]

    # 高さ降順でソート → 似た高さが同じ行に集まり、行高さのばらつきを抑える
    opened.sort(key=lambda img: img.size[1], reverse=True)

    # First Fit Decreasing シェルフパッキング
    # shelf: {"imgs": [(img, x), ...], "remaining": int, "height": int}
    shelves: list[dict] = []

    for img in opened:
        w, h = img.size
        # 収まる既存のシェルフを探す (高さが img.height 以上かつ幅が収まるもの)
        placed = False
        for shelf in shelves:
            if shelf["remaining"] >= w and shelf["height"] >= h:
                x = max_width - shelf["remaining"]
                shelf["imgs"].append((img, x))
                shelf["remaining"] -= w
                placed = True
                break
        if not placed:
            shelves.append({"imgs": [(img, 0)], "remaining": max_width - w, "height": h})

    total_height = sum(s["height"] for s in shelves)
    out = Image.new("RGBA", (max_width, total_height), (0, 0, 0, 0))

    current_y = 0
    for shelf in shelves:
        for img, x in shelf["imgs"]:
            out.paste(img, (x, current_y))
        current_y += shelf["height"]

    out.save(output_path)
    total_imgs = sum(len(s["imgs"]) for s in shelves)
    print(f"✓ {total_imgs} 画像 ({len(shelves)}行) -> {output_path}  ({max_width}x{total_height}px)")


def main() -> None:
    parser = argparse.ArgumentParser(
        description="フォルダ内の全画像を1枚のタイルセットに結合します"
    )
    parser.add_argument("input", help="入力ディレクトリ")
    parser.add_argument("--tile-w", type=int, default=32, help="タイル幅 px (最大幅の基準, default: 32)")
    parser.add_argument("--max-cols", type=int, default=10, help="1行の最大列数 (default: 10)")
    parser.add_argument("--output", "-o", default=None, help="出力ファイルパス (default: <input_dir>/merged_tileset.png)")

    args = parser.parse_args()

    input_path = Path(args.input)
    if not input_path.exists() or not input_path.is_dir():
        print(f"Error: ディレクトリが見つかりません: {input_path}", file=sys.stderr)
        sys.exit(1)

    output_path = args.output or str(input_path / "merged_tileset.png")
    merge_tilesets(str(input_path), output_path, args.max_cols, args.tile_w)


if __name__ == "__main__":
    main()
