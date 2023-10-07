// License:MIT License
// copyright-holders:Hitoshi Iwai

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <msx.h>
#include <msx/gfx.h>


// 定数（インライン展開される）
#define MSX_H_TIMI  0xfd9f
#define VRAM_START  0x1800
#define VRAM_WIDTH  32
#define VRAM_HEIGHT 24
#define CHR_SPACE   0x20


// マクロ（インライン展開される）
#define VPOS(x, y)  (VRAM_START + VRAM_WIDTH * y + x)


// chrptn.asm:CHR_PTN_TBL ラベルの参照(パターンジェネレータテーブル)
extern uint8_t CHR_PTN_TBL[];

// chrptn.asm:CHR_COL_TBL ラベルの参照(カラーテーブル)
extern uint8_t CHR_COL_TBL[];


// 関数のプロトタイプ宣言
void write_text(uint8_t x, uint8_t y, char* text);

void add_bcd(uint16_t addvalue, unsigned char* distaddr, uint8_t distbytes);

void write_bcd(unsigned char* buffaddr, uint8_t buffoffset, unsigned char* distaddr, uint8_t distbytes);


// パターンネームテーブル
uint8_t PTN_NAME_TBL[24*32] = {0};

// スプライトアトリビュートテーブル
uint8_t SPR_ATTR_TBL[32][4] = {0};

// スコアデータ
unsigned char score[3] = {0x00, 0x00, 0x00};

// 画面更新完了フラグ
bool updated = false;


/**
 * 画面更新
 */
void screen_update()
{
    if (updated)
    {
        return;
    }

    #ifndef __INTELLISENSE__
    __asm
    DI
    __endasm;
    #endif

    // パターンネームテーブル更新
    vwrite(PTN_NAME_TBL, 0x1800, 768);

    // スプライトアトリビュートテーブル更新
    vwrite(SPR_ATTR_TBL, 0x1B00, 128);

    #ifndef __INTELLISENSE__
    __asm
    EI
    __endasm;
    #endif

    updated = true;
    return;
}


/**
 * テキスト表示
 * - 構造体に指定されたテキストを仮想画面のx,y座標に書き込む。
 * - 画面端のはみ出し処理はしていないので注意。
 *
 * args:
 * - text_t         text        テキスト表示関数の構造体
 *
 * return:
 * - void
 */
//void write_text(write_text_t arg)
void write_text(uint8_t x, uint8_t y, char* text)
{
    for (uint8_t i = 0; i < strlen(text); i++)
    {
        PTN_NAME_TBL[y*32+x] = text[i];
        x++;
    }
}


/**
 * BCD加算
 * - 対象データに対し引数で与えた16ビットのBCD値を加算する。
 *
 * args:
 * - addvalue       uint16_t    加算値
 * - distaddr       char        加算対象データアドレス
 * - distbytes      uint8_t     加算対象データバイト数
 *
 * return:
 * - void
 */
void add_bcd(uint16_t addvalue, unsigned char* distaddr, uint8_t distbytes)
{
    #ifndef __INTELLISENSE__
__asm
    // リターンアドレスをスキップ
    LD HL,2
    ADD HL,SP                   // SP+2を引数取得開始アドレスに設定

    // 第3引数(加算対象データバイト数)を取得
    LD B,(HL)                   // B <- 加算対象データバイト数
    INC HL
    INC HL

    // 第2引数(加算対象データアドレス)を取得
    LD E,(HL)
    INC HL
    LD D,(HL)
    INC HL
    PUSH DE                     // スタックに退避

    // 第1引数(加算値)を取得
    LD E,(HL)                   // DE <- 加算値
    INC HL
    LD D,(HL)

    // レジスタ設定
    POP HL                      // HL <- 加算対象データアドレス

    // 加算対象データバイト数により処理開始アドレス設定
    DEC B                       // 加算対象データバイト数を1減算(1byte=0、2byte=1、3byte=2)

    LD A,B                      // A <- B
    ADD A,L                     // HL=HL+A
    JR NC,add_bcd_L1
    INC H

add_bcd_L1:
    LD L,A

    // 1〜2桁目の加算
	LD A,E					    // AレジスタにEレジスタの値をロード
    ADD A,(HL)			        // Aレジスタの値に(HL)の値を加算
    DAA						    // Aレジスタの値を内部10進に補正
    						    // 桁溢れした場合はここでキャリーフラグが立つ
    PUSH AF                     // フラグの状態を保存
    LD (HL),A				    // Aレジスタの値を(HL)に格納

    LD A,B                      // Aレジスタに現在の加算対象データバイト数をロード
    OR A                        // ゼロか？
    JR Z,add_bcd_L2             // ゼロならadd_bcd_L2へ
    DEC A                       // 加算対象データバイト数を1減算
    LD B,A                      // Bレジスタに保存

    // 3〜4桁目の加算
    DEC HL                      // 加算対象データアドレスを-1
    POP AF                      // フラグの状態を復帰
    LD A,D					    // AレジスタにDレジスタの値をロード
    ADC A,(HL)			        // Aレジスタの値に(HL)＋キャリーフラグを加算
    						    // 桁溢れした場合はキャリーフラグが立つが無視する
    DAA						    // Aレジスタの値を内部10進に補正
    PUSH AF                     // フラグの状態を保存
    LD (HL),A				    // Aレジスタの値を(HL)に格納

    JR NC,add_bcd_L2            // 桁繰り上がりが無ければadd_bcd_L2へ

    LD A,B                      // Aレジスタに現在の加算対象データバイト数をロード
    OR A                        // ゼロか？
    JR Z,add_bcd_L2             // ゼロならadd_bcd_L2へ

    // 5〜6桁目の加算（桁繰り上がり発生時のみ）
    POP AF                      // フラグの状態を復帰
    OR A                        // フラグクリア
    DEC HL                      // 加算対象データアドレスを-1
    LD A,(HL)                   // 5〜6桁目に1加算
    INC A
    DAA                         // Aレジスタの値を内部10進に補正
    PUSH AF                     // フラグの状態を保存（ダミー）
    LD (HL),A                   // Aレジスタの値を(HL)に格納

add_bcd_L2:
    POP AF

__endasm
    #endif
}


/**
 * BCD値仮想画面表示
 *
 * args:
 * - buffaddr       char        仮想画面アドレス
 * - buffoffset     uint8_t     仮想画面オフセット値
 * - distaddr       char        表示データアドレス
 * - distbytes      uint8_t     表示データバイト数
 *
 * return:
 * - void
 */
void write_bcd(unsigned char* buffaddr, uint8_t buffoffset, unsigned char* distaddr, uint8_t distbytes)
{
    #ifndef __INTELLISENSE__
__asm
    // リターンアドレスをスキップ
    LD HL,2
    ADD HL,SP                   // SP+2を引数取得開始アドレスに設定

    // 第4引数(表示データバイト数)を取得
    LD B,(HL)                   // B -< 表示データバイト数
    INC HL
    INC HL

    // 第3引数(表示データアドレス)を取得
    LD E,(HL)
    INC HL
    LD D,(HL)
    INC HL
    PUSH DE                     // スタックに退避

    // 第2引数(仮想画面オフセット値)を取得
    LD E,(HL)                   // DE <- 仮想画面オフセット値
    INC HL
    LD D,(HL)
    INC HL
    PUSH DE                     // スタックに退避

    // 第1引数(仮想画面アドレス)を取得
    LD E,(HL)                   // DE <- 仮想画面アドレス
    INC HL
    LD D,(HL)
    PUSH DE                     // スタックに退避

    // レジスタ設定
    POP HL                      // HL <- 仮想画面アドレス
    POP DE                      // HL <- 仮想画面オフセット値
    ADD HL,DE                   // HL(仮想画面オフセット値)にDE(仮想画面アドレス)を加算 = 更新対象の仮想画面先頭アドレスを求める

    POP DE                      // DE <- 表示データアドレス

PRTBCD:
    // 表示する桁数分、スペースで埋める
    PUSH BC
    PUSH DE
    PUSH HL

    LD A,B
    ADD A,A                     // 表示文字数を算出 (表示桁*2)
    LD B,A                      // B(繰り返し数) = 表示文字数

PRTBCD_L2:
    LD (HL),$20                 // 表示桁分、スペースを埋める
    INC HL
    DJNZ PRTBCD_L2

    DEC HL
    LD (HL),$30                 // 末尾はゼロ固定表示

    POP HL
    POP DE
    POP BC

    // 数値描画
    LD C,0                      // ゼロ表示フラグ初期化

PRTBCD_L3:
    LD A,(DE)                   // A <- DE(表示データ)
	CALL PUTBCD 			    // データを表示

	INC DE					    // DE=DE+1(＝表示データの次のアドレスが設定される)
    INC HL					    // HL=HL+1(＝表示位置を1つ右に移動)

    DJNZ PRTBCD_L3              // B=B-1、ゼロでなければ繰り返す

	RET

    // 1バイト(2桁)のBCD値描画
PUTBCD:
	// 上1桁の処理
    PUSH AF
    SRL A					    // Aレジスタの値を4回右シフトして、上位4ビットを取り出す
    SRL A
    SRL A
    SRL A
    CALL PUTBCD_L1			    // オフスクリーンバッファにデータ設定

	// 下1桁の処理
    POP AF
    INC HL					    // HLレジスタの値を1加算(＝データ表示位置を1つ右に移動)

PUTBCD_L1:
	// 仮想画面にデータ設定
	AND $0F				        // 上位4ビットをゼロにする(=下位4ビットの値だけ取り出す)
    OR A
    JR NZ,PUTBCD_L2             // 値がゼロ以外の場合はL2へ

    INC C                       // 値がゼロの時はゼロ表示フラグを判定
    DEC C
    RET Z                       // ゼロ表示フラグがOFFの時はゼロ表示せず抜ける

PUTBCD_L2:
    INC C                       // 以降のゼロは表示させるので、ゼロ表示フラグを+1
    ADD A,$30				    // 値にキャラクタコード&H30('0')を加える
    LD (HL),A                   // 仮想画面にデータを設定

    RET

__endasm
    #endif
}


/**
 * 初期設定処理
 *
 * args:
 * - none
 *
 * return:
 * - void
 */
void game_init()
{
    // H.TIMIフック設定
    #ifndef __INTELLISENSE__
    __asm
    DI
    __endasm;
    #endif
    uint8_t *h_timi = (uint8_t *)MSX_H_TIMI;
    uint16_t hook_addr = (uint16_t)&screen_update;
    h_timi[0] = 0xc3; // JP
    h_timi[1] = (uint8_t)(hook_addr & 0xff);
    h_timi[2] = (uint8_t)((hook_addr & 0xff00) >> 8);
    #ifndef __INTELLISENSE__
    __asm
    EI
    __endasm;
    #endif

    // 画面初期化
    set_color(15, 1, 1);
    set_mangled_mode();

    // パターンジェネレータテーブル設定
    vwrite(CHR_PTN_TBL, 0x0000, 0x0800);
    vwrite(CHR_PTN_TBL, 0x0800, 0x0800);
    vwrite(CHR_PTN_TBL, 0x1000, 0x0800);

    // カラーテーブル設定
    vwrite(CHR_COL_TBL, 0x2000, 0x0800);
    vwrite(CHR_COL_TBL, 0x2800, 0x0800);
    vwrite(CHR_COL_TBL, 0x3000, 0x0800);

    // 画面クリア
    fill(VRAM_START, CHR_SPACE, VRAM_WIDTH * VRAM_HEIGHT);
}


/**
 * 画面描画
 */
void make_screen()
{
    write_text(0, 0, "faaf                        aafa");
    write_text(4, 0, "babfaaabaaaaafbfaafaaaab");
    write_text(0, 1, "aafd                        ceba");
    write_text(0, 2, "bff                          aaf");
    write_text(0, 3, "fac                          daf");
    write_text(0, 4, "ad                            fa");
    write_text(0, 5, "a d                           ca");
    write_text(0, 6, "a                            e a");
    write_text(0, 7, "d                              e");
    write_text(0, 8, " d                            e ");
    write_text(0, 9, "b                               ");
    write_text(0,10, "f                              b");
    write_text(0,11, "bd                            bf");
    write_text(0,12, "d                             eb");
    write_text(0,13, "                               e");
    write_text(0,20, "gggggggggggggggggggggggggggggggg");
    write_text(0,22, " SCORE        ROUND      LEFT"); //:GOSUB 2400
//8160 IF GS=1 THEN 8220
    write_text(0,15, "hh");
    write_text(0,16, "hhh");
    write_text(0,17, "ooh");
    write_text(0,18, "  h");
    write_text(0,19, "  h");
//8220 IF GS=5 THEN RETURN
    write_text(29,15, " hh");
    write_text(29,16, "hhh");
    write_text(29,17, "hoo");
    write_text(29,18, "hof");
    write_text(29,19, "hfo");
}


/**
 * ゲーム初期化
 */
void init_screen()
{
    // 画面表示
    make_screen();
}


/**
 * メイン
 */
int main()
{
    // ゲーム初期設定
    game_init();

    // 画面初期化
    init_screen();

    while(true) {
        if (updated)
        {
            updated = false;

            add_bcd(0x0010, score, sizeof(score));
            write_bcd(PTN_NAME_TBL, 6+(22*32), score, sizeof(score));
        }
    }
}
