# 動的に本の情報を切り替える実装を追加する

このままでもそれなりに見栄えはします。しかし、りまりま団の同人誌は6冊もあります。一番思い入れがあるのは1冊目の``ログと情報をレッツ・ラ・まぜまぜ！～ELK Stack で作るBI環境～``という本ですが[^precure]、せっかく作ったのだから本は全部載せたいと思いました。

[^precure]: インプレスR＆Dさんから商業化されて、シリーズのまとめ本にも入れてもらって、なんか連載とかいう新しい企画に載せてもらって2018年に改版もされてBooth販売もされて…。この本くんに何回励まされたことか…またこういうものを作りたいです。

## データの持ち方を考える

載せる情報は次の通りです。

<!-- ここに入れる -->

問題はこの情報をどこで管理するか？ということです。このとき思いついた手段をあげます。

- RDB
- NoSQL
- クラウドのサービス（Amazon Redshift系列）
- Google SpreadSheet
- JSONなどのテキストにしてassetsディレクトリに置いておく

データの管理と言えばRDBやNoSQLですね。NoSQLであればJSON形式でデータのやりとりができるので、Vue.jsで情報を利用するのに役立ちそうです。ただし、これらはホスティングにお金がかかります。サービスを使うのも、サーバーを借りるのもお金がかかる点では一緒です。さらに、情報量も項目数も少ないので、サーバー費用が少し勿体無いと思いました。お金ないし（重要）。
次にGoogle SpreadSheetですが、今回の情報の形式だと横に長いSpreadSheetができそうだと思いました。関連URLには複数のURLとタイトルがひも付きますし、KUSO Pointも同様です。これは記載することが面倒そうなのでやめました。また、Google Sheets APIは利用したことがないので、この利用方法をあれこれ模索できるだけの時間はないなと判断しました。
というわけで、消去法でJSONに本の情報を記載し、``assets``ディレクトリから読み込む方式としました。書影は``assets``ディレクトリに全て配置し、本ごとにディレクトリを分けることにしました。ファイル名を全て同じ名前で保存しておけば、``v-bind``でパスを割り当てるときにディレクトリ名を切り替えるだけで良いからです。

## データを作成する

データの持ち方は決まりました。次はいよいよデータを作成します。

### 載せたい情報をスプレッドシートにまとめる

はじめは自分でJSONを書こうとしました。結論として5分であきらめました。Googleの[JSON Style Guide](https://google.github.io/styleguide/jsoncstyleguide.xml)（``https://google.github.io/styleguide/jsoncstyleguide.xml``）などを読んではみたのですが、今までの業務の中で1からJSONを設計することがなかったので、「ちょっと何言ってるかわからない」状態になってしまいました。
個人開発ですし、迷惑がかかるのは将来の自分だけ、という言い訳と共に、はじめはCSVからJSONを作ってくれるWebサービスを利用したほうがいいだろうという判断をしました。綺麗なJSONのデータ構造よりも、クソアプリカレンダーとこのコミックマーケット95の新刊を入稿に間に合うことのほうがはるかに大事だったからです。

そこで、まずはCSVを作ることにしました。[Google SpreadSheet](https://docs.google.com/spreadsheets/d/16NGDz_8Xl4hAzjCPNHjx5pVN8cxNFOsoPngeLrF633A/edit?usp=sharing)（``https://docs.google.com/spreadsheets/d/16NGDz_8Xl4hAzjCPNHjx5pVN8cxNFOsoPngeLrF633A/edit?usp=sharing``）に必要な項目を記載します。JSONへどのように加工されるかがわからなかったため、1項目に複数の子項目が紐づいている場合はIDを降ることにしました。

![作成したスプレッドシート](C95-vue-and-nuxt/images/chapter5)

https://github.com/MofuMofu2/portfolio-vue/pull/26

### りまりま団の同人誌リスト

- https://docs.google.com/spreadsheets/d/16NGDz_8Xl4hAzjCPNHjx5pVN8cxNFOsoPngeLrF633A/edit?usp=sharing