# Panasonic SC-EN27 / SA-EN27 紅外線遙控器資料庫

這份文件整理以 ESP8266 自製紅外線發射器，重建 Panasonic SC-EN27／SA-EN27 音響遙控器時，實際找到的紅外線指令。

原廠遙控器型號：`N2QAGB000038`

> 本專案的目標是保存已測得的紅外線資料，並讓 ESP8266 能取代原廠遙控器的常用功能。表格中的功能名稱依實際測試時觀察到的音響反應整理。

## 目前成果

- 已找到 **5 組有效的 Device / Subdevice 組合**。
- 已整理 **29 個按鍵／功能指令**。
- 使用 Panasonic 48-bit 紅外線協定。
- 已找到 MUSIC PORT、CD、TUNER/BAND、SOUND EQ、音量、數字鍵、POWER、DISPLAY、SLEEP、CLOCK/TIMER 等功能。
- CD STOP 與 PLAY MODE 尚未復現；測試時沒有可用 CD 進行完整驗證。

## 硬體與軟體環境

| 項目 | 設定 |
|---|---|
| 開發板 | Difi WF8266RDF ESP8266 IoT Learning Board |
| Arduino IDE | Arduino IDE 2 |
| 紅外線函式庫 | `IRremoteESP8266` |
| 紅外線發射 | `IR_E` = GPIO0（D3） |
| 紅外線接收 | `IR_R` = GPIO4（D2） |
| Serial Monitor | 115200 baud |
| 協定 | Panasonic，48 bits |
| Device | 160（所有已找到的指令） |

### 腳位提醒

Difi WF8266RDF 的紅外線腳位：

- `IR_E`（IR Sender）：GPIO0 / D3
- `IR_R`（IR Receiver）：GPIO4 / D2
- `IRSW` 是紅外線障礙物感測相關腳位，不是發射腳位。

## 指令資料

所有指令的 Device 均為 `160`。`IR Data` 是交給 `IRremoteESP8266` 的 `sendPanasonic64()` 使用的 48-bit 值。

`Repeat` 是測試紀錄中的 `sendPanasonic64(code, 48, repeats)` 第三個參數：

- `0`：初始訊號發送一次，不額外重複。
- `1`：初始訊號之外，再額外重複一次。

表格先忠實保留測試時記錄的 Repeat 值。若要製作正式遙控器，尤其 POWER 這種切換式按鍵，建議逐鍵確認使用 `0` 或 `1` 的實際效果，不要直接把所有按鍵統一成相同 Repeat。

### Subdevice 0 — AMP/0308

| 按鍵／功能 | OBC | IR Data | Repeat |
|---|---:|---|---:|
| MUSIC PORT | 154 | `0x40040500595C` | 1 |
| Volume + | 32 | `0x400405000401` | 1 |
| Volume - | 33 | `0x400405008481` | 1 |
| Live Virtualizer | 48 | `0x400405000C09` | 1 |

### Subdevice 4 — AMP/0309

| 按鍵／功能 | OBC | IR Data | Repeat |
|---|---:|---|---:|
| TUNER/BAND | 164 | `0x400405202500` | 0 |

### Subdevice 10 — CD/0303

| 按鍵／功能 | OBC | IR Data | Repeat |
|---|---:|---|---:|
| OPEN/CLOSE | 1 | `0x4004055080D5` | 0 |
| CD | 10 | `0x400405505005` | 1 |

### Subdevice 16 — Panasonic SC Series 線索

| 按鍵／功能 | OBC | IR Data | Repeat |
|---|---:|---|---:|
| SOUND EQ | 131 | `0x40040508C1CC` | 0 |

### Subdevice 28 — AMP/0518

| 按鍵／功能 | OBC | IR Data | Repeat |
|---|---:|---|---:|
| 1 | 16 | `0x400405380835` | 0 |
| 2 | 17 | `0x4004053888B5` | 0 |
| 3 | 18 | `0x400405384875` | 0 |
| 4 | 19 | `0x40040538C8F5` | 0 |
| 5 | 20 | `0x400405382815` | 0 |
| 6 | 21 | `0x40040538A895` | 0 |
| 7 | 22 | `0x400405386855` | 0 |
| 8 | 23 | `0x40040538E8D5` | 0 |
| 9 | 24 | `0x400405381825` | 0 |
| 0 | 25 | `0x4004053898A5` | 0 |
| POWER | 61 | `0x40040538BC81` | 1 |
| Skip/Search - | 73 | `0x4004053892AF` | 0 |
| Skip/Search + | 74 | `0x40040538526F` | 0 |
| DISPLAY | 85 | `0x40040538AA97` | 0 |
| ≥10 | 132 | `0x40040538211C` | 0 |
| PGM/CLEAR | 138 | `0x40040538516C` | 0 |
| CLOCK/TIMER ADJ | 149 | `0x40040538A994` | 1 |
| SLEEP | 150 | `0x400405386954` | 1 |
| TIMER SET/CHECK | 155 | `0x40040538D9E4` | 1 |
| ALBUM/CHANNEL SELECT - | 252 | `0x400405383F02` | 0 |
| ALBUM/CHANNEL SELECT + | 253 | `0x40040538BF82` | 0 |

## Subdevice 掃描紀錄

| Device | Subdevice | 結果 | 備註 |
|---:|---:|---|---|
| 160 | 0 | 找到功能 | AMP/0308：MUSIC PORT、音量、Live Virtualizer |
| 160 | 4 | 找到功能 | AMP/0309：TUNER/BAND |
| 160 | 8 | 無反應 | 已完成掃描 |
| 160 | 10 | 找到功能 | CD/0303：OPEN/CLOSE、CD |
| 160 | 16 | 找到功能 | 找到 SOUND EQ（OBC 131） |
| 160 | 18 | 無反應 | 已完成掃描 |
| 160 | 20 | 無反應 | 已完成掃描 |
| 160 | 28 | 找到功能 | AMP/0518：數字鍵、POWER、DISPLAY、SLEEP、TIMER 等 |
| 160 | 194 | 無反應 | AMP/0367，已完成掃描 |

## 尚未復現／尚待補充

- **CD STOP**：尚未找到／驗證。測試時沒有可用 CD 進行實際播放狀態測試。
- **PLAY MODE**：尚未找到／驗證。測試時沒有可用 CD 進行實際播放狀態測試。
- 其他按鍵若原廠遙控器另有列出，仍可日後按功能模式繼續補測。

## 測試方法與注意事項

1. 先確認發射腳位使用 GPIO0（IR_E），接收腳位使用 GPIO4（IR_R）。
2. 使用 `IRremoteESP8266` 的 Panasonic 48-bit 發送方式。
3. 將發射器對準音響紅外線接收窗。
4. 測試時記錄音響實際反應、Device、Subdevice、OBC、IR Data 與 Repeat。
5. 序列監控視窗中的 `[RECEIVED]` 是 ESP8266 接收端收到自己發出的紅外線訊號（loopback）的證據；它本身不等於音響已接受指令。功能名稱與狀態應以音響本身的實際反應為準。
6. 音響可能依目前輸入來源（FM、CD、MUSIC PORT）而對同一指令有不同反應。遇到依模式而異的功能，請分別記錄測試模式。
7. POWER 是切換式功能時，重複發送可能造成連續切換。正式使用前應確認 Repeat 次數；可優先測試 Repeat 0。

## 來源與致謝

- 按鍵資料由使用者透過 Difi WF8266RDF ESP8266 開發板、`IRremoteESP8266` 與 Panasonic SC-EN27 音響實際測試整理。
- JP1 / One For All 設定碼資料用於縮小搜尋範圍；最終按鍵對應以本專案的實機測試紀錄為準。
- 本專案的硬體測試、協定分析、按鍵窮舉與資料整理由使用者與 ChatGPT 協作完成。

---

**目前資料版本：** 依據 2026-11-05 前後的實機測試紀錄整理。後續發現新指令時，請同步更新 README 與 `ir-codes.csv`。
