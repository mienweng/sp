這份 README.md 文件展示了兩個版本的程式碼，分別是使用和未使用 Mutex 同步機制的情況下進行銀行帳戶存款和提款操作。未使用 Mutex 的版本可能導致競態條件，最終帳戶餘額不可預測；而使用 Mutex 的版本則保證了操作的原子性和結果的正確性。這突顯了在多線程環境中使用同步機制的重要性，以確保數據的一致性和避免競態條件帶來的問題。