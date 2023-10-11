// License:MIT License
// copyright-holders:Hitoshi Iwai

#include <stdint.h>
#include <string.h>

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
 * BCD→10進変換転送
 *
 * args:
 * - distaddr       char        書き込み先アドレス
 * - distoffset     uint16_t     書き込み先オフセット値
 * - dataaddr       char        表示データアドレス
 * - databytes      uint8_t     表示データバイト数
 *
 * return:
 * - void
 */
void wrtbcdtodec(unsigned char* distaddr, uint16_t distoffset, unsigned char* dataaddr, uint8_t databytes)
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

    // 表示する桁数分、スペースで埋める
    PUSH BC
    PUSH DE
    PUSH HL

    LD A,B
    ADD A,A                     // 表示文字数を算出 (表示桁*2)
    LD B,A                      // B(繰り返し数) = 表示文字数

write_bcd_L2:
    LD (HL),$20                 // 表示桁分、スペースを埋める
    INC HL
    DJNZ write_bcd_L2

    DEC HL
    LD (HL),$30                 // 末尾はゼロ固定表示

    POP HL
    POP DE
    POP BC

    // 数値描画
    LD C,0                      // ゼロ表示フラグ初期化

write_bcd_L3:
    LD A,(DE)                   // A <- DE(表示データ)
	CALL put_bcd			    // データを表示
	INC DE					    // DE=DE+1(＝表示データの次のアドレスが設定される)
    INC HL					    // HL=HL+1(＝表示位置を1つ右に移動)
    DJNZ write_bcd_L3           // B=B-1、ゼロでなければ繰り返す

	RET

    // 1バイト(2桁)のBCD値描画
put_bcd:
	// 上1桁の処理
    PUSH AF
    SRL A					    // Aレジスタの値を4回右シフトして、上位4ビットを取り出す
    SRL A
    SRL A
    SRL A
    CALL put_bcd_L1			    // オフスクリーンバッファにデータ設定

	// 下1桁の処理
    POP AF
    INC HL					    // HLレジスタの値を1加算(＝データ表示位置を1つ右に移動)

put_bcd_L1:
	// 仮想画面にデータ設定
	AND $0F				        // 上位4ビットをゼロにする(=下位4ビットの値だけ取り出す)
    OR A
    JR NZ,put_bcd_L2            // 値がゼロ以外の場合はL2へ

    INC C                       // 値がゼロの時はゼロ表示フラグを判定
    DEC C
    RET Z                       // ゼロ表示フラグがOFFの時はゼロ表示せず抜ける

put_bcd_L2:
    INC C                       // 以降のゼロは表示させるので、ゼロ表示フラグを+1
    ADD A,$30				    // 値にキャラクタコード&H30('0')を加える
    LD (HL),A                   // 仮想画面にデータを設定

    RET

__endasm
    #endif
}
