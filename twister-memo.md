Midi Fighter Twisterのメモ
====
URL
* http://www.midifighter.com/#twister-features
* https://store.djtechtools.com/midi-fighter-twister#downloads_and_support
  * ↑MF Utility(設定用アプリ)をダウンロードすることができる

以後、factory reset状態で使うことを前提とする
* MF Utility(設定用アプリ)を使うと、各ノブのcontrol numberの割り当てや、ノブを押したときの挙動(momentary/alternateなど)を変えることができるので注意

BANK切り替え
----
筐体左右についている3つのボタンの真ん中でBANK0～3を切り替えることができる

* 初期状態はBANK0
* 右側面真ん中ボタンを押すと、順にBANK1→BANK2→BANK3に切り替わる。
* 左側面真ん中ボタンを押すと、順にBANK2→BANK1→BANK0に切り替わる

バンクが切り替わるときは、次の2つのCCメッセージが送られてくる
* 例: bank0→bank1に切り替わるときに送られてくるメッセージ
  * channel:3,  control_number:0, control_value:0x00
  * channel:3,  control_number:1, control_value:0x7f
  * (表示されるBANK1の値が0x7fに設定され、非表示になるBANK0の値が0x00になる)

ノブ回転
----
Twisterには16個の物理ノブが配置されているが、ファーム的には64個の仮想的なノブが用意されている。
BANK0～3を切り替えることで64個のノブを操作できるようになっている。

ノブを回した際に、CCメッセージが送られてくる。

channelは1を使用

control_number割り当て
* BANK0 : 左上から右に順に、control_number 0～15
* BANK1 : 左上から右に順に、control_number 16～31
* BANK2 : 左上から右に順に、control_number 32～47
* BANK3 : 左上から右に順に、control_number 48～61

control_valueの範囲は0～127、中心は63

※ノブの値を表示するインジケータは、MF Utility(設定用アプリ)を使って、dot, bar, blended barを選択することができる

ノブの押下
----
ノブは押し込み操作が可能。CCメッセージが送られてくる。

* channelは2を使用
* control_number割り当てはノブ回転と同じ
* control_valueは押したとき127、離したとき0

PCからの制御
----
CCメッセージをTwisterに送信すると、ノブの現在値、LEDの色、点滅パターン、表示BANKを制御可能
* アプリ起動時に、アプリ側の量と、フィジカルコントローラの回転量を強制的に合わせることができる。
* 他のMIDIフィジカルコントローラではできないことが多いので、これができるTwisterはかなりいい感じ。

channel0
* ノブの現在値を設定
* control_numberはノブ回転時と割り当ては同じ

channel1
* ノブ下側についているLEDインジケータ色の設定
* control_numberはノブ回転時と割り当ては同じ
* 0～127でHUEを設定できる。白は設定できない

channel2
* 点滅パターン制御
* (今回のプラグインでは制御していないのであしからず…)

channel3
* BANKの切り替え制御
* control_number：0～3, control_value:0x7fを送信すると、BANK0～3に切り替えることができる
