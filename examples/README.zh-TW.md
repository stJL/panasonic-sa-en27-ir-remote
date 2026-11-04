# Arduino 範例程式

本資料夾收錄 SC-EN27 紅外線逆向研究過程使用的程式。這些程式是研究與測試工具，並非已完成的日常遙控器韌體。

## 需求

- ESP8266 相容的 Arduino Board Package。
- Difi WF8266RDF 開發板（或接線正確的 ESP8266、紅外線發射器與接收器）。
- [IRremoteESP8266](https://github.com/crankyoldgit/IRremoteESP8266) Arduino 函式庫。
- Serial Monitor 設定為 **115200 baud**。

本專案使用的 Difi WF8266RDF 腳位：

- 紅外線發射端（`IR_E`）：GPIO0 / D3。
- 紅外線接收端（`IR_R`）：GPIO4 / D2。

硬體說明與研究參考請見根目錄的[繁體中文 README](../README.zh-TW.md)。

## 程式說明

### `01_ir_nec_loopback_test.ino`

使用簡單 NEC 訊號進行發送／接收迴路測試，確認發射端能送出紅外線訊號，接收端能捕捉並解碼。此程式不測試 Panasonic 指令，也不能證明音響會接受指令。

### `02_panasonic_48bit_loopback_test.ino`

建立並發送 Panasonic 48-bit 測試訊號，再檢查本機接收端能否解碼相同資料。在開始控制音響前，先驗證 Panasonic 的基本收發流程。

### `03_panasonic_obc_scanner.ino`

主要逆向研究工具，功能包括：

- 掃描 OBC 0–255。
- 修改 Device 與 Subdevice。
- 手動發送指定 OBC，或逐碼前進／後退。
- 暫停、繼續、停止及調整掃描位置。
- 調整掃描間隔與重複發送次數。
- 不受目前掃描設定影響，獨立發送已知的 MUSIC PORT 指令。
- 接收 Panasonic 訊號，解碼並顯示 Device、Subdevice、OBC。

Serial Monitor 指令請以程式檔案開頭的註解為準。需要輸入數字的命令必須按 Enter（Line Ending 請設為 Newline 或 Both NL & CR）；單一字母命令會立即執行。

## 測試注意事項

1. 開始掃描前，先確認 ESP8266 發射／接收路徑正常。
2. 將發射端對準音響，掃描時觀察音響顯示幕及實際反應。
3. `[RECEIVED]` 只代表 ESP8266 接收端捕捉到訊號，不代表音響已執行指令。
4. 每個候選指令都要記錄輸入模式（FM、MUSIC PORT、CD 等）及音響實際反應。
5. POWER、SLEEP、TIMER 等可能改變設備狀態的指令要特別小心；音響出現非預期反應時應立即停止掃描。

掃描器可能發送大量實驗性指令，請只在自己擁有或獲授權測試的設備上使用。
