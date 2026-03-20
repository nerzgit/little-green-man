# ゲーム制作用の便利ツール

uvによる仮想環境を構築して利用してください

セットアップ:  
 cd tools
uv sync # .venv 作成 + Pillow インストール

実行方法（2通り）:

# uv run で直接

uv run make_tileset.py player.png --crop-w 100 --crop-h 100 --tile-w 32 --tile-h 32

# スクリプトとして（uv sync 後）

uv run make-tileset player.png --crop-w 100 --tile-w 32

uv sync さえ叩けば .venv と依存関係が全部揃います。
.venv と uv.lock は .gitignore に入れておくと良いです。

# 画像素材を32px X 32px のタイルセットに分解調整する

```
// 32 x 32 ずつのタイルにし、最大幅は5列（ディレクトリ指定可）
uv run  make_tileset.py 画像パス --tile-w 32 --tile-h 32 --cols 5 -o 出力パス
```

```
// ディレクトリ直下のタイル画像を1枚の画像に結合(タイルの最大列)
uv run merge_tilesets.py 画像ディレクトリ --max-cols 10 -o 出力パス
```
