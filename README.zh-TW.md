# Panasonic SC-EN27 / SA-EN27 紅外線遙控器資料

本專案整理在重建 Panasonic SC-EN27／SA-EN27 迷你音響遙控器時，透過實機測試找到的紅外線（IR）指令。

研究中參考的原廠遙控器型號：`N2QAGB000038`。

> **注意：** 本專案為獨立的逆向研究，並非 Panasonic 官方文件。按鍵名稱與指令對應是根據實際測試整理，可能仍有錯誤或不完整之處。如果你發現錯誤、有修正建議，或找到其他指令，非常歡迎提出 Issue 或 Pull Request。請盡量附上音響型號、輸入模式、指令資料及實際觀察到的反應。

English version: [README.md](README.md)

## 專案進度

- 目前整理出 **5 組 Device 160 的 Subdevice**，共 **29 組按鍵／功能指令**。
- 目前觀察到的協定為 Panasonic 48-bit。
- 已找到 MUSIC PORT、TUNER/BAND、CD 控制、音量、音效、數字鍵、DISPLAY、SLEEP、TIMER 等功能。
- CD STOP 和 PLAY MODE 尚未復現；相關播放測試期間沒有可用的 CD。

表格中的功能名稱是本專案測試時觀察到的結果。同一指令可能會依音響目前的輸入來源或操作狀態而有不同反應。

## 硬體與軟體環境

| 項目 | 設定 |
|---|---|
| 開發板 | Difi WF8266RDF ESP8266 IoT Learning Board |
| 開發環境 | Arduino IDE 2 |
| 紅外線函式庫 | `IRremoteESP8266` |
| 紅外線發射 | `IR_E` = GPIO0（D3） |
| 紅外線接收 | `IR_R` = GPIO4（D2） |
| Serial Monitor | 115200 baud |
| 紅外線協定 | Panasonic，48 bits |
| Device | 160（目前整理的所有指令） |

### 開發板腳位提醒

本專案使用的 Difi WF8266RDF 腳位：

- `IR_E` 是紅外線發射端，GPIO0 / D3。
- `IR_R` 是紅外線接收端，GPIO4 / D2。
- `IRSW` 與紅外線障礙物感測功能有關，不是紅外線發射腳位。

## 程式碼與實作方式

[`examples/`](examples/) 資料夾收錄這次驗證硬體及逆向研究時使用的 Arduino 程式：

| 程式 | 用途 |
|---|---|
| [`01_ir_nec_loopback_test.ino`](examples/01_ir_nec_loopback_test.ino) | 使用 NEC 測試訊號確認紅外線發射與接收硬體。 |
| [`02_panasonic_48bit_loopback_test.ino`](examples/02_panasonic_48bit_loopback_test.ino) | 驗證 Panasonic 48-bit 編碼、發送及本機接收。 |
| [`03_panasonic_obc_scanner.ino`](examples/03_panasonic_obc_scanner.ino) | 主要研究工具：掃描 OBC、手動發送指令、控制掃描進度並解碼接收訊號。 |
| [`examples/README.md`](examples/README.md) | 編譯環境、建議測試順序及結果判讀說明（英文）。 |
| [`examples/README.zh-TW.md`](examples/README.zh-TW.md) | 範例程式說明（繁體中文）。 |

### 逆向研究流程

1. 先用已知的 NEC 測試訊號確認紅外線發射與接收硬體正常。
2. 再用產生的 Panasonic 48-bit 測試資料，確認協定編碼與接收解碼正常。
3. 使用 OBC 掃描程式設定 Device 和 Subdevice，逐一測試 OBC，同時觀察音響本身的反應。
4. 記錄音響實際反應，以及對應的 Device、Subdevice、OBC 和 48-bit 資料。

掃描程式會將 JP1 格式的位元順序轉換成 `IRremoteESP8266` 的 `sendPanasonic64()` 所使用的順序，並計算 checksum。公開程式碼與指令表，是希望其他研究者可以重現測試方法、修正既有判讀，並補充新的研究結果。

**目前範圍：** Repo 收錄的是硬體測試與紅外線逆向研究程式；尚未包含具有實體按鍵、顯示器或自訂操作介面的完整獨立遙控器韌體。

## 紅外線指令資料

以下所有指令的 Device 都是 `160`。`IR Data` 是搭配 `IRremoteESP8266` 的 `sendPanasonic64()` 使用的 48-bit 數值。

### Subdevice 0 — AMP/0308

| 按鍵／功能 | OBC | IR Data |
|---|---:|---|
| MUSIC PORT | 154 | `0x40040500595C` |
| 音量增加 | 32 | `0x400405000401` |
| 音量降低 | 33 | `0x400405008481` |
| Live Virtualizer | 48 | `0x400405000C09` |

### Subdevice 4 — AMP/0309

| 按鍵／功能 | OBC | IR Data |
|---|---:|---|
| TUNER/BAND | 164 | `0x400405202500` |

### Subdevice 10 — CD/0303

| 按鍵／功能 | OBC | IR Data |
|---|---:|---|
| OPEN/CLOSE | 1 | `0x4004055080D5` |
| CD | 10 | `0x400405505005` |

### Subdevice 16 — Panasonic SC 系列研究線索

| 按鍵／功能 | OBC | IR Data |
|---|---:|---|
| SOUND EQ | 131 | `0x40040508C1CC` |

### Subdevice 28 — AMP/0518

| 按鍵／功能 | OBC | IR Data |
|---|---:|---|
| 1 | 16 | `0x400405380835` |
| 2 | 17 | `0x4004053888B5` |
| 3 | 18 | `0x400405384875` |
| 4 | 19 | `0x40040538C8F5` |
| 5 | 20 | `0x400405382815` |
| 6 | 21 | `0x40040538A895` |
| 7 | 22 | `0x400405386855` |
| 8 | 23 | `0x40040538E8D5` |
| 9 | 24 | `0x400405381825` |
| 0 | 25 | `0x4004053898A5` |
| POWER | 61 | `0x40040538BC81` |
| Skip/Search − | 73 | `0x4004053892AF` |
| Skip/Search + | 74 | `0x40040538526F` |
| DISPLAY | 85 | `0x40040538AA97` |
| >=10 | 132 | `0x40040538211C` |
| PGM/CLEAR | 138 | `0x40040538516C` |
| CLOCK/TIMER ADJ | 149 | `0x40040538A994` |
| SLEEP | 150 | `0x400405386954` |
| TIMER SET/CHECK | 155 | `0x40040538D9E4` |
| ALBUM/CHANNEL SELECT DOWN | 252 | `0x400405383F02` |
| ALBUM/CHANNEL SELECT UP | 253 | `0x40040538BF82` |

機器可讀取的指令表另存於 [`ir-codes.csv`](ir-codes.csv)。

## Subdevice 研究紀錄 — 歡迎一起補充

下表只是本專案在目前測試環境中探索過的 Device 160 Subdevice 紀錄，**不是相容性清單，也不代表某個 Subdevice 沒有用途或不受音響支援**。

部分組合在本專案的測試過程中，尚未觀察到可確認的音響反應。結果可能受測試條件、輸入模式、音響狀態或功能判讀影響。非常歡迎其他研究者測試並分享不同結果；即使結果與本專案不同，也很有參考價值。

| Device | Subdevice | 研究紀錄 |
|---:|---:|---|
| 160 | 0 | 已找到功能；與 AMP/0308 相關。 |
| 160 | 4 | 已找到 TUNER/BAND；與 AMP/0309 相關。 |
| 160 | 8 | 本專案曾測試，尚未確認對應功能；歡迎進一步研究。 |
| 160 | 10 | 已找到 OPEN/CLOSE、CD；與 CD/0303 相關。 |
| 160 | 16 | 已找到 SOUND EQ；由 Panasonic SC 系列研究線索追查。 |
| 160 | 18 | 本專案曾測試，尚未確認對應功能；歡迎進一步研究。 |
| 160 | 20 | 本專案曾測試，尚未確認對應功能；歡迎進一步研究。 |
| 160 | 28 | 已找到數字鍵、POWER、DISPLAY、Skip/Search、Album/Channel 與 TIMER 相關功能；與 AMP/0518 相關。 |
| 160 | 194 | 曾依 AMP/0367 線索測試，尚未確認對應功能；歡迎進一步研究。 |

若你測試了上述組合，歡迎分享以下資訊：

- 音響型號，以及已知的地區版本。
- 當時的輸入模式（FM、CD、MUSIC PORT 等）。
- Device、Subdevice、OBC 與完整 IR Data。
- 音響實際反應（包括沒有明顯變化、顯示幕文字，或依狀態而異的反應）。
- 相關測試條件或指令順序。

## 尚未復現

- **CD STOP**：尚未找到或驗證；測試期間沒有可用的 CD 進行播放測試。
- **PLAY MODE**：尚未找到或驗證；測試期間沒有可用的 CD 進行播放測試。

這些是仍待研究的項目，不代表它們無法復現。

## 測試注意事項

1. 確認開發板腳位：發射端 GPIO0（`IR_E`），接收端 GPIO4（`IR_R`）。
2. 使用 `IRremoteESP8266` 的 Panasonic 48-bit 格式。
3. 將紅外線發射端對準音響的紅外線接收窗。
4. 請分開記錄音響實際反應與 ESP8266 接收端的輸出。
5. ESP8266 顯示 `[RECEIVED]`，可以證明本機接收端捕捉到發出的訊號（loopback）；但單靠這個訊息，**不能證明音響已接受或執行指令**。
6. 同一指令在 FM、CD、MUSIC PORT 等不同輸入模式下，可能有不同效果。回報測試結果時請註明輸入模式。

## 研究參考資料與致謝

以下連結記錄本次逆向研究使用過的背景資料與線索。它們是**研究參考，不代表某個萬用遙控器設定碼或其他 Panasonic 型號的指令必然適用於 SC-EN27**。`ir-codes.csv` 中的紅外線對應資料，來自本專案對 SC-EN27／SA-EN27 的實機測試。

### 1. Panasonic SC-EN27 原廠操作說明書

- [Panasonic SC-EN27 Operating Instructions — RQT8578-1B（ManualsDump 鏡像）](https://manualsdump.com/en/manuals/panasonic-sc-en27/2804/1)：確認型號與操作說明；文件列出 SC-EN27 系統及 SA-EN27 主機。
- [第 2 頁：配件與原廠遙控器](https://manualsdump.com/en/manuals/panasonic-sc-en27/2804/2)：列出 SC-EN27 使用的遙控器零件編號 `N2QAGB000038`。
- [第 3 頁：CD 操作](https://manualsdump.com/en/manuals/panasonic-sc-en27/2804/3)：核對 CD、OPEN/CLOSE 及播放相關功能。
- [第 5 頁：定時器與其他功能](https://manualsdump.com/en/manuals/panasonic-sc-en27/2804/5)：核對 CLOCK/TIMER、SLEEP、SOUND EQ、Live Virtualizer 及 DISPLAY。
- [第 6 頁：控制按鍵指南](https://manualsdump.com/en/manuals/panasonic-sc-en27/2804/6)：原廠遙控器按鍵配置與功能名稱。
- [另一份說明書鏡像（ManualsLib）](https://www.manualslib.com/manual/119612/Panasonic-Sc-En27.html)。

以上是第三方網站提供的操作說明書副本，並非 Panasonic 官方網站上的下載連結。本專案只引用其型號與功能資訊，不重新散布說明書內容。

### 2. JP1 / One For All 研究資料

- [One For All / JP1 依品牌列出的 AMP 設定碼](https://www.hifi-remote.com/ofa/codes-amp.shtml)：列出 Panasonic 音響的設定碼候選，包括 `0039`、`0308`、`0309`、`0518`。設定碼是特定萬用遙控器內的預設碼組，**不是可以直接發送的紅外線資料**。
- [JP1 AMP 設定碼索引](https://www.hifi-remote.com/devices-amp.shtml)：在其遙控器資料庫中，將 `0308`、`0309`、`0518` 等 AMP 設定碼列為 Panasonic／Technics。
- [JP1 CD 設定碼索引](https://www.hifi-remote.com/devices-cd.shtml)：列出 Panasonic／Technics 的 CD 設定碼 `0303`。
- [JP1 討論：Panasonic Audio 設定碼與 Device 160 Subdevice](https://www.hifi-remote.com/forums/viewtopic.php?p=15038)：記錄 Audio/0303 → 160.10、Audio/0308 → 160.0、Audio/0309 → 160.4、Audio/0367 → 160.194、Audio/0518 → 160.28 等歷史對應。這些資料用來縮小搜尋範圍，之後仍由本專案在 SC-EN27 上獨立測試。
- [JP1 討論：Panasonic 離散電源指令](https://www.hifi-remote.com/forums/viewtopic.php?t=2341)：討論 Panasonic Device 160 及多種可能的 Subdevice。文中的離散開／關範例來自其他設備，不能直接假設適用 SC-EN27。
- [JP1 討論：Comcast 遙控器與 Panasonic SC-BT200](https://www.hifi-remote.com/forums/viewtopic.php?t=13162)：這是另一台 Panasonic SC 系列音響。該討論對 Combo2／SC Series EQ 的研究，啟發我們追查其他 Subdevice；其中的 EFC 數值**不是本專案公布的 SC-EN27 指令碼**。
- [JP1 Panasonic 協定參考](https://www.hifi-remote.com/wiki/index.php/Panasonic)：Panasonic／Kaseikyo 協定家族及資料欄位的背景說明。

`AMP/0039` 也出現在設定碼清單中，但它同時被多個品牌共用；本專案尚未找到能可靠對應 SC-EN27 的 Device/Subdevice，因此沒有把它列為已可用的指令組合。

### 3. 紅外線協定與程式庫

- [IRremoteESP8266 原始碼與專案](https://github.com/crankyoldgit/IRremoteESP8266)：本專案在 ESP8266 上發送及接收紅外線訊號所使用的 Arduino 函式庫。
- [IRremoteESP8266 `IRsend` API 文件](https://crankyoldgit.github.io/IRremoteESP8266/doxygen/html/classIRsend.html)：說明測試程式及掃描器使用的 `encodePanasonic()` 與 `sendPanasonic64()`。

### 4. Difi WF8266RDF 開發板參考

- [Yungger：So Easy MicroPython — ESP8266 Difi WF8266RDF IoT Learning Board](https://yungger.medium.com/so-easy-micropython-esp8266-difi-wf8266rdf-iot-learning-board-and-its-demos-937e1f3affd)：文章中的紅外線範例使用接收腳位 `4`、發射腳位 `0`，協助辨認本專案開發板的 IR 接收與發射連接；之後也在本專案實際使用的開發板上完成驗證。

### 如何理解這些參考資料

- Panasonic 說明書用來核對音響原本提供的按鍵與功能；它沒有提供本 Repo 所列的 IR Hex／OBC 指令表。
- JP1／One For All 資料協助辨認候選協定、設定碼、Device/Subdevice 組合及可能的功能。有些論壇案例使用的是其他 Panasonic 型號。
- 本 Repo 公布的 Device／Subdevice／OBC／IR Data 對應是**本專案的實測觀察**；在其他使用者獨立重現之前，仍應視為實驗性資料。
- ESP8266 接收端出現 `[RECEIVED]` 代表本機接收到迴路訊號，不足以單獨證明音響已接受或執行指令。

### 致謝

感謝 Panasonic 說明書鏡像的整理者、JP1／RemoteMaster 社群、IRremoteESP8266 專案維護者，以及 Difi WF8266RDF 文章作者。本專案的逆向研究、測試與文件整理由專案維護者與 ChatGPT 協作完成。

## 錯誤修正與社群貢獻

本文件可能有錯誤，歡迎提出修正或補充。若你發現按鍵名稱不正確、缺少指令、Subdevice 不同，或某個功能只在特定模式下出現，歡迎建立 Issue 或提交 Pull Request。請提供足夠細節，讓其他人可以重現你的觀察。

---

本文件依據約 2026-11-05 前後的實機測試紀錄整理；隨著更多設備與操作模式被測試，內容可能持續更新。
