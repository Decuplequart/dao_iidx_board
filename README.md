# DAO Controller Board

DAO系IIDXコントローラーの既存ボタン、ターンテーブルセンサー、LEDハーネスを再利用し、USB HIDおよびBluetooth Low Energy HIDへの対応を目指す交換用コントローラー基板です。

本リポジトリには、KiCadで作成した回路図・PCBデータ、プロジェクト固有ライブラリ、およびJLCPCB向け製造データ生成に必要な設定を収録します。

Repository: https://github.com/Decuplequart/dao_iidx_board

> [!WARNING]
> この基板は現在 **Rev.1の試作段階** です。実機での完全な動作確認、長時間運転、全LED同時点灯時の温度評価、USB/BLEファームウェア検証は完了していません。製造・実装・使用は自己責任で行ってください。

## 主な機能

- 7鍵入力
- START / SELECT入力
- 2相ターンテーブルセンサー入力（RD1 / RD2）
- USB 2.0 Full-Speed HID対応を想定
- Bluetooth Low Energy HID対応を想定
- DAO系単色ボタンLED 9系統の個別制御
- RESETボタン
- MODE / PAIRボタン
- SWD書き込み端子
- 8 GPIOの拡張端子
- USB Type-C給電
- モバイルバッテリー給電によるBLE運用を想定

## 設計方針

この基板は内蔵バッテリーを搭載せず、USB Type-Cからの5V給電に統一しています。

- 有線使用時: PCとのUSB通信および給電
- 無線使用時: BLE通信および外部モバイルバッテリーからの給電

内蔵LiPo、充電回路、昇圧回路を省略することで、回路の簡略化、安全性の向上、保守性の改善を狙っています。

## ハードウェア構成

### マイコン

- Raytac `MDBT50Q-1MV2`
- Nordic Semiconductor `nRF52840`搭載
- USB / BLE対応
- チップアンテナ内蔵
- SWD経由で初回書き込み可能

JLCPCB/LCSC部品番号:

```text
C5118826
```

この部品は通常在庫ではなく、事前注文が必要になる場合があります。また、ブートローダーやアプリケーションが未書き込みの可能性があるため、SWD書き込みを前提としてください。

### LEDドライバ

- Texas Instruments `TLC6C598PWR` × 2
- 8ch DMOSオープンドレイン出力
- 2個をカスケード接続し、9系統を使用

DAO系単色ボタンLEDの実測電流は1灯あたり約40～50mAでした。LED電源は5V、ドライバ出力はローサイドシンク方式です。

```text
U2 DRAIN0～7 -> LED_K1～LED_K8
U3 DRAIN0    -> LED_K9
```

### 電源

```text
USB Type-C VBUS
  -> VBUS_5V
     -> MDBT50Q VDDH / VBUS
     -> TLC6C598 × 2
     -> KEY_LED_5V
     -> SCR_5V
     -> DISH_LED_5V
```

- 内蔵バッテリーなし
- USB Type-C受電用CC抵抗: 5.1kΩ × 2
- キーLED共通電源は太配線で分配
- 裏面GNDゾーンを使用

## GPIO割り当て

| 機能 | nRF52840 GPIO |
|---|---|
| K1 | P0.02 |
| K2 | P0.03 |
| K3 | P0.04 |
| K4 | P0.05 |
| K5 | P0.28 |
| K6 | P0.29 |
| K7 | P0.30 |
| START | P0.00 |
| SELECT | P0.01 |
| MODE_PAIR | P0.06 |
| RD1 | P0.13 |
| RD2 | P0.14 |
| LED_DATA | P0.15 |
| LED_CLK | P0.16 |
| LED_LATCH | P0.17 |
| RESET | P0.18 / nRESET |

### 拡張GPIO

`GPIO_EXPANSION`端子へ次の8本を引き出しています。

```text
EXP0 -> P0.19
EXP1 -> P0.20
EXP2 -> P0.21
EXP3 -> P0.22
EXP4 -> P0.23
EXP5 -> P0.24
EXP6 -> P0.25
EXP7 -> P0.26
```

将来のPS2対応や追加I/Oへの利用を想定していますが、現時点では用途・ファームウェアとも未確定です。

## コネクター

| 機能 | 種類 |
|---|---|
| DISH_LED | JST XH 2極、2.50mm、垂直 |
| KEY1～KEY7 | JST XH 4極、2.50mm、垂直 |
| START / SELECT | JST XH 4極、2.50mm、垂直 |
| DISH_SENSOR | JST XH 4極、2.50mm、垂直 |
| SWD | 1×4、2.54mmピンヘッダー |
| GPIO_EXPANSION | 2×5、2.54mmピンヘッダー |

キー・START・SELECT用4極コネクターの基本構成:

```text
Pin 1: スイッチ入力
Pin 2: LED制御端子（LOWで点灯）
Pin 3: KEY_LED_5V
Pin 4: GND
```

実際の既存ハーネスを接続する前に、必ず導通とピン順を確認してください。

## SWD端子

```text
J3 Pin 1: +3V3 / VTref
J3 Pin 2: SWDIO
J3 Pin 3: SWDCLK
J3 Pin 4: GND
```

J-Link、nRF52840 DK、またはJLCPCBのプログラミングサービスを使った初回書き込みを想定しています。

## 基板仕様

```text
基板外形: 80.00 mm × 57.00 mm
層数: 2層
推奨板厚: 1.6 mm
銅厚: 1 oz
固定穴: 4個、5.5 mm NPTH
```

固定穴中心位置は、基板左上を `(0, 0)` とした場合、次のとおりです。

```text
左上: ( 4.5, 15.0 ) mm
右上: (75.5, 15.0 ) mm
左下: ( 4.5, 45.0 ) mm
右下: (75.5, 45.0 ) mm
```

## アンテナ領域の注意

`MDBT50Q-1MV2`のアンテナKEEP OUT内には、表裏とも次を配置しないでください。

- 銅箔ゾーン
- 配線
- ビア
- 部品
- 金属スペーサー
- ネジや金属板

Gerber出力後、アンテナ領域からGNDベタが正しく抜けていることを必ず確認してください。

## 主なJLCPCB/LCSC部品

| リファレンス | 部品 | LCSC番号 |
|---|---|---|
| C1, C3, C4, C6 | 100nF / 16V / 0402 | C1525 |
| C2, C5 | 4.7uF / 16V / 0603 | C19666 |
| D1 | 1SS404 / SOD-323 | C2847422 |
| D2 | SS14 / SMA | C2480 |
| R1 | 0Ω / 0402 | C17168 |
| R2, R3 | 5.1kΩ / 0402 | C25905 |
| R4, R5 | 10kΩ / 0402 | C25744 |
| R6, R7 | 1kΩ / 0402 | C11702 |
| SW1, SW2 | SMDタクトスイッチ | C49234124 |
| U1 | MDBT50Q-1MV2 | C5118826 |
| U2, U3 | TLC6C598PWR | C2863406 |
| USB-C | TYPE-C-31-M-12 | C165948 |
| J1 | JST B2B-XH-A | C158012 |
| J3 | 1×4 2.54mmピンヘッダー | C5116483 |
| J4 | 2×5 2.54mmピンヘッダー | C492422 |
| J5～J14 | JST B4B-XH-A | C144395 |

> [!NOTE]
> 在庫、価格、Basic/Extended区分、Standard PCBA対応状況は変動します。発注直前にJLCPCBの部品ページと実装プレビューで再確認してください。

## リポジトリ構成

```text
.
├─ *.kicad_pro              KiCadプロジェクト
├─ *.kicad_sch              回路図
├─ *.kicad_pcb              PCB
├─ *.kicad_sym              プロジェクト固有シンボル
├─ *.pretty/                プロジェクト固有フットプリント
├─ *.3dshapes/              3Dモデル
├─ fp-lib-table             フットプリントライブラリ設定
├─ sym-lib-table            シンボルライブラリ設定
└─ README.md
```

3Dモデルは製造そのものには必須ではありません。Gerber、ドリル、BOM、CPLの内容を優先してください。

## 製造データの生成

KiCad用`JLCPCB Tools`プラグインを使用する場合:

1. PCBエディターでDRCを実行
2. エラー0、未配線0を確認
3. GNDゾーンを再計算
4. JLCPCB Toolsを開く
5. 各実装部品へLCSC番号を割り当て
6. 固定穴とテストポイントをBOM/POSから除外
7. `Generate`を実行

PCBA発注に必要な主なファイル:

```text
Gerber + Drill ZIP
BOM.csv
CPL / Positions.csv
```

発注時はJLCPCBのプレビューで、少なくとも次を確認してください。

- U1、U2、U3の1番ピン方向
- D1、D2のカソード方向
- USB Type-Cの向き
- JST-XHコネクターの向き
- タクトスイッチの位置
- SMT部品の回転角
- PTH/NPTH穴
- アンテナKEEP OUT
- 表裏シルク

## ファームウェア

ファームウェアは別途開発予定です。

想定機能:

- USB HIDゲームコントローラー
- BLE HIDゲームコントローラー
- 7鍵、START、SELECT入力
- ターンテーブル2相入力
- LED制御
- MODE / PAIR操作
- 将来の拡張GPIO利用

初回書き込みはSWDを使用します。JLCPCBへ書き込みを依頼する場合は、対象デバイス、SWD端子定義、書き込み対象HEX、Erase/Program/Verify手順を明記してください。

## 既知の注意事項

- Rev.1は未検証です。
- DAO系コントローラーの世代やモデルにより、ハーネスのピン配列が異なる可能性があります。
- LED電流は実機で約40～50mA/灯を観測しています。
- 9灯同時点灯時はLEDだけで約360～450mAになる可能性があります。
- モバイルバッテリーによっては低負荷時に自動停止する場合があります。
- U1は事前注文、Standard PCBA、X線検査が必要になる場合があります。
- U1が未書き込み品の場合、SWD経由の初回書き込みが必要です。
- 未確認の状態でPC、コントローラー、既存ハーネスへ接続しないでください。

## ライセンス

本リポジトリの内容は、特に明記がない限り **MIT License** の下で公開します。

詳細は[`LICENSE`](LICENSE)を参照してください。

## Disclaimer

本プロジェクトは非公式の個人製作物です。DAO、JLCPCB、LCSC、Raytac、Nordic Semiconductor、Texas Instruments、JSTその他の各社とは関係ありません。各名称および商標は、それぞれの権利者に帰属します。
