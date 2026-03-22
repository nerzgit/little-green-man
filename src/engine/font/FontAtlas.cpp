#include "FontAtlas.hpp"

#include "../graphics/Texture.hpp"

// stb_truetype はヘッダーオンリーライブラリ。
// このマクロを定義した翻訳単位（.cpp）でだけ実装コードが展開される。
// 複数の .cpp
// でインクルードするときは、この行を書くのは必ず1ファイルだけにすること。
#define STB_TRUETYPE_IMPLEMENTATION
#include <stb_truetype.h>

#include <fstream>
#include <stdexcept>
#include <vector>

// ============================================================
// FontAtlas とは何か？
//
// ゲームで文字を描くとき、毎フレーム「フォントファイルから
// この文字を読み込んで…」とやると処理が重くなる。
//
// そこでゲーム起動時に「使いそうな文字を全部まとめて
// 1枚の画像（テクスチャ）に焼き込んでおく」。
// 描画時はその画像から必要な文字だけ切り抜いて貼る。
// この「文字がぎっしり詰まった1枚の画像」がフォントアトラス。
//
//   ┌──────────────────────────────────┐
//   │ あいうえお…  アイウエオ…  一二三… 	　　　　　│
//   │ ABCDEFGabc…  漢字漢字漢字…      　　　　　　│
//   └──────────────────────────────────┘
//             ↑ これが atlasSize×atlasSize ピクセルの画像
// ============================================================

FontAtlas::FontAtlas(const std::string& fontPath, float fontSize, int atlasSize)
    : fontSize_(fontSize) {
	// ============================================================
	// ① TTF ファイルをメモリに丸ごと読み込む
	// ============================================================
	//
	// std::ios::binary … バイナリモードで開く（テキストとして解釈しない）
	// std::ios::ate   … ファイルを開いた瞬間にカーソルを末尾へ移動する
	//                   （末尾のカーソル位置 = ファイルサイズ、という仕掛け）
	std::ifstream file(fontPath, std::ios::binary | std::ios::ate);
	if (!file.is_open())
		throw std::runtime_error("FontAtlas: cannot open font: " + fontPath);

	// tellg() で「今いる位置 = 末尾 = ファイルサイズ」を取得し、
	// その分だけメモリを確保する
	std::vector<uint8_t> fontData(file.tellg());

	// seekg(0) で先頭に戻ってから全バイト読み込む
	file.seekg(0);
	file.read(reinterpret_cast<char*>(fontData.data()), fontData.size());

	// ============================================================
	// ② アトラスに焼き込む文字の「範囲」を定義する
	// ============================================================
	//
	// 文字には「コードポイント」という番号が割り当てられている（Unicode）。
	// たとえば「A」= 0x0041、「あ」= 0x3042 など。
	// 連続する範囲をまとめて指定することで効率よく処理できる。
	//
	// ─────────────────────────────────────────────────────────────
	//  開始コード  文字数   内容
	// ─────────────────────────────────────────────────────────────
	//  0x0020       96    ASCII 印字可能文字
	//                     スペース(0x20) 〜 チルダ '~'(0x7E) の 96文字
	//                     ※ 0x7E - 0x20 + 1 = 95... に見えるが
	//                       stb_truetype は end 込みで num_chars 個扱うため 96
	//                       が正しい
	//
	//  0x3040       96    ひらがな
	//                     小文字ぁ(0x3041) の 1つ前から始まり 96文字分
	//                     （Unicode ひらがなブロックは 0x3040〜0x309F =
	//                     96コード）
	//
	//  0x30A0       96    カタカナ
	//                     (Unicode カタカナブロックは 0x30A0〜0x30FF =
	//                     96コード)
	//
	//  0x4E00    20992    CJK 統合漢字（いわゆる漢字）
	//                     「一」(0x4E00) 〜 「鿿」(0x9FFF) まで
	//                     0x9FFF - 0x4E00 + 1 = 0x5200 = 20992 文字
	// ─────────────────────────────────────────────────────────────
	struct Range {
		uint32_t start;
		int      count;
	};
	const Range ranges[] = {
	  {0x0020, 96},    // ASCII    （スペース〜~）
	  {0x3040, 96},    // ひらがな（ぁ〜ゞ）
	  {0x30A0, 96},    // カタカナ（゠〜ヿ）
	  {0x4E00, 20992}, // CJK統合漢字（一〜鿿）← 漢字2万字
	};
	constexpr int kRangeCount = 4;

	// ============================================================
	// ③ stb_truetype に渡すグリフ情報配列を準備する
	// ============================================================
	//
	// stbtt_packedchar … 「この文字は画像のどこに配置されたか」を
	//                     stb_truetype が書き込む構造体。
	//
	// 4つの範囲を「1本のフラットな配列」で管理する。
	// 各範囲は配列内のどこから始まるかを offset で区切る。
	//
	//  packed 配列のイメージ：
	//  [ ASCII×96 | ひらがな×96 | カタカナ×96 | 漢字×20992 ]
	//    offset=0   offset=96     offset=192    offset=288
	//
	int totalChars = 0;
	for (auto& r : ranges)
		totalChars += r.count;

	std::vector<stbtt_packedchar> packed(totalChars);

	// stbtt_pack_range … stb_truetype に「この範囲を焼いて」と伝える構造体
	stbtt_pack_range packRanges[kRangeCount];
	int              offset = 0;
	for (int i = 0; i < kRangeCount; ++i) {
		packRanges[i].font_size                        = fontSize;
		packRanges[i].first_unicode_codepoint_in_range = ranges[i].start;
		packRanges[i].num_chars                        = ranges[i].count;
		// この範囲の結果を packed 配列のどの位置に書き込むか、をポインタで渡す
		packRanges[i].chardata_for_range = packed.data() + offset;
		// nullptr = コードポイントを連番で自動生成（個別配列は使わない）
		packRanges[i].array_of_unicode_codepoints = nullptr;
		offset += ranges[i].count;
	}

	// ============================================================
	// ④ stb_truetype で全文字を 1枚のグレースケール画像に焼き込む
	// ============================================================
	//
	// bitmap … atlasSize×atlasSize ピクセルのグレースケール画像バッファ。
	//          uint8_t 1バイト = 1ピクセル（0=透明, 255=文字が白く光る）
	std::vector<uint8_t> bitmap(atlasSize * atlasSize);

	stbtt_pack_context ctx;

	// PackBegin の引数：
	//   &ctx          … 作業用コンテキスト（状態を保持する入れ物）
	//   bitmap.data() … 書き込み先バッファ
	//   atlasSize     … 画像の横幅（ピクセル）
	//   atlasSize     … 画像の縦幅（ピクセル）
	//   0             …
	//   ストライド（行の末尾に余白バイトがある場合に指定。0=詰め詰め） 1 …
	//   文字同士の隙間（パディング）。1ピクセル空けることで滲みを防ぐ nullptr
	//   … メモリアロケータ（nullptr=デフォルト malloc 使用）
	stbtt_PackBegin(&ctx, bitmap.data(), atlasSize, atlasSize, 0, 1, nullptr);

	// オーバーサンプリング倍率（X方向, Y方向）
	//   1, 1 … 1倍＝ドット絵風のくっきりした文字
	//   2以上… スーパーサンプリングでアンチエイリアスがかかりなめらかになる
	//           ただし atlas の使用面積が倍率の二乗倍に増えるので注意
	stbtt_PackSetOversampling(&ctx, 1, 1);

	// 実際に文字をビットマップへ配置・ラスタライズする
	stbtt_PackFontRanges(&ctx, fontData.data(), 0, packRanges, kRangeCount);

	stbtt_PackEnd(&ctx);

	// ============================================================
	// ⑤ グレースケール画像 → RGBA画像 に変換して GPU テクスチャを作る
	// ============================================================
	//
	// GPU（OpenGL など）は RGBA = 4チャンネルのテクスチャを期待することが多い。
	// 1ピクセルあたり [R, G, B, A] の 4バイトで表す。
	//
	//  * 4 … 1ピクセル = 4バイト（R, G, B, A）なのでバッファサイズも4倍
	std::vector<uint8_t> rgba(atlasSize * atlasSize * 4);

	for (int i = 0; i < atlasSize * atlasSize; ++i) {
		// RGB はすべて 255（白）に固定する。
		// 描画時にシェーダーで好きな色を掛け算すれば任意の色の文字が出せる。
		//   i*4+0 = R チャンネル
		//   i*4+1 = G チャンネル
		//   i*4+2 = B チャンネル
		rgba[i * 4 + 0] = rgba[i * 4 + 1] = rgba[i * 4 + 2] = 255;

		// A（アルファ = 透明度）だけ元のグレースケール値をそのまま使う。
		// グレースケールが 0（文字のない箇所）→ 完全透明
		// グレースケールが 255（文字の輪郭）   → 完全不透明
		//   i*4+3 = A チャンネル
		rgba[i * 4 + 3] = bitmap[i];
	}

	texture_ = std::make_unique<Texture>(atlasSize, atlasSize, rgba.data());

	// ============================================================
	// ⑥ 「この文字はアトラス画像のどこにある？」を辞書（glyphs_）に登録する
	// ============================================================
	//
	// テクスチャの UV 座標は 0.0〜1.0 の正規化された値。
	// stb_truetype が返すのはピクセル座標なので、
	//   UV = ピクセル座標 / 画像サイズ
	// で変換する。毎回割り算するのは遅いので逆数（1/atlasSize）を先に計算しておく。
	const float inv = 1.0f / static_cast<float>(atlasSize);

	offset = 0;
	for (int i = 0; i < kRangeCount; ++i) {
		for (int j = 0; j < ranges[i].count; ++j) {
			const auto& pc = packed[offset + j];
			Glyph       g;

			// アトラス画像内でこの文字が占める矩形の左上・右下を UV 座標に変換
			// pc.x0, pc.y0 = 左上のピクセル座標
			// pc.x1, pc.y1 = 右下のピクセル座標
			g.uvMin = {pc.x0 * inv, pc.y0 * inv};
			g.uvMax = {pc.x1 * inv, pc.y1 * inv};

			// ポリゴン（四角形）を何ピクセル大きさで描くか
			g.size = {float(pc.x1 - pc.x0), float(pc.y1 - pc.y0)};

			// ベアリング（カーソル位置からのオフセット）
			// 文字によってはベースラインより下に飛び出す（例: 'g', 'p'）ので
			// そのズレを xoff / yoff で補正する
			g.bearing = {pc.xoff, pc.yoff};

			// 次の文字へカーソルを何ピクセル進めるか（文字幅 + 字間）
			g.advance = pc.xadvance;

			// コードポイント番号をキーにして辞書に登録
			// 例: 'A'(0x41), 'あ'(0x3042) などをキーに検索できるようになる
			glyphs_[ranges[i].start + j] = g;
		}
		offset += ranges[i].count;
	}

	// ============================================================
	// ⑦ 1行の高さ（lineHeight_）を計算する
	// ============================================================
	//
	// フォントの縦方向の各パーツ：
	//
	//   ─────────── ベースライン
	//        ↑  ascent  （ベースラインから文字の一番上まで。正の値）
	//        ↓  descent （ベースラインから文字の一番下まで。負の値）
	//        ↓  lineGap （次の行との間に入れる余白）
	//
	//   1行の高さ = ascent - descent + lineGap
	//   （descent が負なので引くと足し算になる）
	//
	// stb_truetype が返す値は「フォント内部単位」という独自の整数。
	// scale = stbtt_ScaleForPixelHeight(..., fontSize) で変換係数を求め、
	// 掛け算してピクセル値に直す。
	stbtt_fontinfo info;
	stbtt_InitFont(&info, fontData.data(), 0);
	const float scale = stbtt_ScaleForPixelHeight(&info, fontSize);
	int         ascent, descent, lineGap;
	stbtt_GetFontVMetrics(&info, &ascent, &descent, &lineGap);
	lineHeight_ = float(ascent - descent + lineGap) * scale;
}

// ============================================================
// getGlyph — 文字コードから Glyph 情報を取り出す
// ============================================================
//
// codepoint … 取得したい文字の Unicode コードポイント（例: 'A'=0x41,
// 'あ'=0x3042） 戻り値    … Glyph 構造体へのポインタ。辞書に存在しない文字なら
// nullptr。
const FontAtlas::Glyph* FontAtlas::getGlyph(uint32_t codepoint) const {
	auto it = glyphs_.find(codepoint);
	return it != glyphs_.end() ? &it->second : nullptr;
}
