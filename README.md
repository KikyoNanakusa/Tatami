# VimLikeHotkeyWindowManager
Windowsでウィンドウを移動させる時に`Windows+arrow_hoge`を押すのが嫌だったので作りました
Vimっぽいホットキーでウィンドウを移動させられます

# 使い方
## タスクトレイ
本アプリケーションはタスクトレイアプリです。終了する際はタスクトレイのアイコンを右クリックしてメニューから終了してください  
タスクトレイ内に有効/無効を切り替えるボタンもあります

## Hotkeys
基本的に、フォーカスのあるウィンドウに対して作用します  
たまに効かないウィンドウがあるかもしれません
`Alt+Shift+H`: ウィンドウを左方向へ移動  
`Alt+Shift+J`: ウィンドウを下方向へ移動  
`Alt+Shift+K`: ウィンドウを上方向へ移動  
`Alt+Shift+L`: ウィンドウを右方向へ移動  
`Alt+Shift+G`: ウィンドウを最大化  
`Alt+Shift+D`: ウィンドウを最小化  
`Alt+Shift+U`: 最小化したウィンドウを元に戻す  

左右にウィンドウを動かす場合、ウィンドウが画面端にある && モニターが複数ある場合はモニターをまたいで移動します  
最大がされているウィンドウに対して移動を試みた場合、ウィンドウサイズが画面サイズの半分になります  
自分で使っててほしい機能が増え次第拡張します  

# 既知の不具合
任意のウィンドウに対して、位置を変更する関係上発生する問題があります
モニターをまたいでウィンドウを移動した際に、モニター間でDPIが同一でない場合DPIスケーリングがかかり、ウィンドウサイズが変化してします
だれかいい解決策知ってたら教えてください

# 免責事項
本プログラムを使用して発生したいかなる不利益も責任を取りません
WindowsAPIでOSに直でホットキーを追加しているのでもしかすると何かあるかもしれません