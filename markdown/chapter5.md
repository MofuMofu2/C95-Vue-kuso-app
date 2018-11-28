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

## #23 JSONデータを作成する

データの持ち方は決まりました。次はいよいよデータを作成します。

### 載せたい情報をスプレッドシートにまとめる

はじめは自分でJSONを書こうとしました。結論として5分であきらめました。Googleの[JSON Style Guide](https://google.github.io/styleguide/jsoncstyleguide.xml)（``https://google.github.io/styleguide/jsoncstyleguide.xml``）などを読んではみたのですが、今までの業務の中で1からJSONを設計することがなかったので、「ちょっと何言ってるかわからない」状態になってしまいました。
個人開発ですし、迷惑がかかるのは将来の自分だけ、という言い訳と共に、はじめはCSVからJSONを作ってくれるWebサービスを利用したほうがいいだろうという判断をしました。綺麗なJSONのデータ構造よりも、クソアプリカレンダーとこのコミックマーケット95の新刊を入稿に間に合うことのほうがはるかに大事だったからです。

そこで、まずはCSVを作ることにしました。[Google SpreadSheet](https://docs.google.com/spreadsheets/d/16NGDz_8Xl4hAzjCPNHjx5pVN8cxNFOsoPngeLrF633A/edit?usp=sharing)（``https://docs.google.com/spreadsheets/d/16NGDz_8Xl4hAzjCPNHjx5pVN8cxNFOsoPngeLrF633A/edit?usp=sharing``）に必要な項目を記載します。JSONへどのように加工されるかがわからなかったため、1項目に複数の子項目が紐づいている場合はIDを降ることにしました。

![作成したスプレッドシート](C95-vue-and-nuxt/images/chapter5)

### CSVをJSONに変換する

重要な情報が入ったCSVではないので、Webサービスを利用して手軽に変換したいという観点でWebサービスを探しました。その中でも[SQLIFY](https://sqlify.io/)というWebサービスの画面がわかりやすそうだったため、早速変換してみることにしました。CSVまたはJSONをアップロードすると、SQL・JSON・CSVのどれかに変換することができるサービスです。

![SQLIFYのWebサービスにアクセスしたところ](C95-vue-and-nuxt/images/chapter5/sqlify_top.png)

今回はSpreadSheetをCSV形式でダウンロードした後、SQLIFYへアップロードしました。アップロード後にCSVの行とJSONのカラム名の対応を修正し、データ型を決めることができます。手作業で数値と文字列の変換をする必要がないため、これはかなり便利だと思いました。

![CSVをアップロードしたあと](C95-vue-and-nuxt/images/chapter5/sqlify_convert.png)

変換後のJSONですが、次のようなテキストがブラウザに表示されます。

```json
[
{"overviews_promotion":"ふざけたタイトル","overviews_id":1,"booth_url":"https://booth.pm/ja/items/490460","book_title":"ログと情報をレッツ・ラ・まぜまぜ！～ELK Stack で作るBI環境～","book_info_url":"http://rimarimadan.hatenablog.com/entry/2017/04/12/技術書典2_もふもふちゃんの戦い履歴","book_info_title":"技術書典2 もふもふちゃんの戦い履歴","book_info_id":"1","book_id":"1","book_description":"Elasticsearch社のプロダクトであるElasticsearch・Logstash・Kibanaを用いてログを分析してみようという本です。各ミドルウェアのインストールからKibanaでグラフを描画するまでの道のりを一通り網羅しつつまとめました。インプレスR＆Dさんから商業化もされています。"},
{"overviews_promotion":"なにも伝わってこない表紙","overviews_id":2,"booth_url":"","book_title":"","book_info_url":"http://amzn.asia/d/be7bGtk","book_info_title":"商業本","book_info_id":"2","book_id":"","book_description":""},
{"overviews_promotion":"布教本のはずなのにまさかの省略記法を使っている","overviews_id":3,"booth_url":"","book_title":"","book_info_url":"","book_info_title":"","book_info_id":"","book_id":"","book_description":""},
{"overviews_promotion":"本編と全く関係ないペンギンの表紙","overviews_id":1,"booth_url":"https://mofu-mofu.booth.pm/items/666604","book_title":"データを加工する技術","book_info_url":"http://rimarimadan.hatenablog.com/entry/2017/10/17/技術書典3_告知","book_info_title":"技術書典3 告知","book_info_id":"1","book_id":"2","book_description":"『ログと情報をレッツ・ラ・まぜまぜ！～ELK Stack で作るBI環境～』の続編に当たる本です。今回はデータを収集・加工する工程に重点をおいて解説しています。Logstashを使ってALBのログを取得、filterプラグインを用いてデータを扱いやすい形に加工する方法について紹介しています。加えて、LogstashとFluentdの設定を見比べつつ、結局どちらを選択すればいいんだろう？ということも考えています。こちらは『Introduction Elastic Stack6』と合わせて商業化されました。"},
{"overviews_promotion":"章が変わったときにページを詰める方法がわからず、むりやりペンギンの写真を詰め込んだ根性","overviews_id":2,"booth_url":"","book_title":"","book_info_url":"http://rimarimadan.hatenablog.com/entry/2017/10/23/技術書典3に参加しました","book_info_title":"技術書典3に参加しました","book_info_id":"2","book_id":"","book_description":""},
{"overviews_promotion":"魔法使いプリキュア！に似ているというだけで採用された表紙のフォント","overviews_id":3,"booth_url":"","book_title":"","book_info_url":"http://amzn.asia/d/e3iTGAa","book_info_title":"商業本","book_info_id":"3","book_id":"","book_description":""},
{"overviews_promotion":"考えるのが面倒臭くて適当に作った表紙","overviews_id":1,"booth_url":"https://mofu-mofu.booth.pm/items/965753","book_title":"Introduction Elastic Stack6","book_info_url":"http://rimarimadan.hatenablog.com/entry/2018/04/17/技術書典4にサークル参加します！","book_info_title":"技術書典4にサークル参加します！","book_info_id":"1","book_id":"3","book_description":"Elastic Stackがバージョン6になったので、バージョン6で追加された機能やElasticsearch APIの操作方法の知見を寄せ集めました。導入するだけでKibanaのBI画面作成までを実施してくれるFilebeat Modulesの使用例はバージョン6の目玉機能なのでは、と個人的に考えています。もちろんKibanaのUIがアップデートされたため、それに追従してKibanaを操作しつつデータを可視化する知見も入れ込みました。こちらは『データを加工する技術』と合わせて商業化されています。"},
{"overviews_promotion":"この本のリポジトリのgit logを取得しているので駆け込み原稿であることがもろバレである","overviews_id":2,"booth_url":"","book_title":"","book_info_url":"http://rimarimadan.hatenablog.com/entry/2018/04/23/技術書典4に参加しました%E3%80%82","book_info_title":"技術書典4に参加しました。","book_info_id":"2","book_id":"","book_description":""},
{"overviews_promotion":"ちなみに（ユーザー名はマスキングして）そのまま商業化されている","overviews_id":3,"booth_url":"","book_title":"","book_info_url":"http://amzn.asia/d/e3iTGAa","book_info_title":"商業本","book_info_id":"3","book_id":"","book_description":""},
{"overviews_promotion":"遊び紙の白い部分をこすると（無駄に）レモンのにおいがする。いいにおい。","overviews_id":1,"booth_url":"https://mofu-mofu.booth.pm/items/965770","book_title":"ひよこエンジニアに送るお仕事サバイバルガイド","book_info_url":"http://rimarimadan.hatenablog.com/entry/2018/04/17/技術書典4にサークル参加します！","book_info_title":"技術書典4にサークル参加します！","book_info_id":"1","book_id":"4","book_description":"新卒4年目時点で感じた「こういう風に学べばよかった」「こういう風に質問すればよかった/されると嬉しい」「こんな人にはなりたくないなあ」「どうやって調べたらいいんだろう」的なエモポエムに対する感想＋もふもふちゃんのエモポエム集です。自分への戒め＋1年目で知ってたらもっと変わったかもな〜〜〜他の人にも教えたろ！みたいないい迷惑の知見を布教するつもりで書いた本その1です。"},
{"overviews_promotion":"KUSOみたいな人がいて「きぃ〜〜〜〜〜っ！」となったので書いた","overviews_id":2,"booth_url":"","book_title":"","book_info_url":"http://rimarimadan.hatenablog.com/entry/2018/04/23/技術書典4に参加しました%E3%80%82","book_info_title":"技術書典4に参加しました。","book_info_id":"2","book_id":"","book_description":""},
{"overviews_promotion":"なぜか「あなたひよこちゃうやろ！」って人の元に多くの本が旅立っていった","overviews_id":3,"booth_url":"","book_title":"","book_info_url":"https://ec.toranoana.jp/tora_r/ec/item/040030674964","book_info_title":"とらのあな通販","book_info_id":"3","book_id":"","book_description":""},
{"overviews_promotion":"仕事で全然技術的なことができず、本のネタが作れないのにコミケにでることになった","overviews_id":1,"booth_url":"https://mofu-mofu.booth.pm/items/965778","book_title":"ひよこエンジニアに送るサバイバルガイド 〜深淵の闇を見た大人編〜","book_info_url":"http://rimarimadan.hatenablog.com/entry/2018/07/17/コミックマーケット94に出場します","book_info_title":"コミックマーケット94に出場します","book_info_id":"1","book_id":"5","book_description":"『ひよこエンジニアに送るお仕事サバイバルガイド』では書けなかったようなブラックなネタ（温めておいた）だけを集めて書いたのですが、このときはもう転職していたのですっかりやる気がなくなって…はないです。もふもふちゃんが仕事で出会ってやだったな〜〜〜と思った人のアンチパターン事例を集めました。自分が仲間入りしないように頑張ります。"},
{"overviews_promotion":"新刊がないのは寂しいので「このKUSSSSSSSO〜〜〜〜〜」という気持ちを起こしてみた","overviews_id":2,"booth_url":"","book_title":"","book_info_url":"","book_info_title":"","book_info_id":"","book_id":"","book_description":""},
{"overviews_promotion":"本が薄くてぺにゃってなる。ペニャペニャの本です","overviews_id":3,"booth_url":"","book_title":"","book_info_url":"","book_info_title":"","book_info_id":"","book_id":"","book_description":""},
{"overviews_promotion":"このためだけに高校数学の参考書を買った。しかも解説がわからなかったのでわかる人に教えてもらった。高校生かな？","overviews_id":1,"booth_url":"https://ec.toranoana.jp/tora_r/ec/item/040030674965","book_title":"第1回 Webエンジニア基礎学力試験 問題と解説","book_info_url":"http://rimarimadan.hatenablog.com/entry/2018/10/10/技術書典5に参加できるか怪しかったけど参加で","book_info_title":"技術書典5に参加できるか怪しかったけど参加できた話","book_info_id":"","book_id":"6","book_description":"Webアプリケーション関連の用語、色々あって毎回調べ直すのめんどくさ…そうだ用語集を作ろう！と思って原稿を始めました。進めるうちに「用語集だと結局読まない気がする」と思い始め、問題を考えて回答を作れば理解も深まるのでは…という結論に至りました。理解が深まったか…はちょっと自信がありませんが、学校の定期テストを考えている先生はほんと大変だなという気持ちになりました。通販は大好きなとらのあなさんにお願いしました。個人的にもお世話になっているので、とらのあな専売シールをつけてもらいたかったんです。"},
{"overviews_promotion":"過去問題集を参考に表紙を作った。見開きで作ったら閉じ方向を間違えて作り直しになった。","overviews_id":2,"booth_url":"","book_title":"","book_info_url":"https://techbookfest.org/event/tbf05/circle/41000006","book_info_title":"技術書典のサークルページ","book_info_id":"","book_id":"","book_description":""}
]
```

このままではみづらいので、1冊目の本に絞ってデータを表示します。先ほどidを降った部分のせいで、無駄なJSONの塊ができていることがわかります。本当は1つの``overviews``に``overviews_id``と``overviews_promotion``が3つ紐づくようにしたいのです。このままでは利用することができません。仕方がないので、手作業で全て修正しました。

```json
[
  {
    "overviews_promotion": "ふざけたタイトル",
    "overviews_id": 1,
    "booth_url": "https://booth.pm/ja/items/490460",
    "book_title": "ログと情報をレッツ・ラ・まぜまぜ！～ELK Stack で作るBI環境～",
    "book_info_url": "http://rimarimadan.hatenablog.com/entry/2017/04/12/技術書典2_もふもふちゃんの戦い履歴",
    "book_info_title": "技術書典2 もふもふちゃんの戦い履歴",
    "book_info_id": "1",
    "book_id": "1",
    "book_description": "Elasticsearch社のプロダクトであるElasticsearch・Logstash・Kibanaを用いてログを分析してみようという本です。各ミドルウェアのインストールからKibanaでグラフを描画するまでの道のりを一通り網羅しつつまとめました。インプレスR＆Dさんから商業化もされています。"
  },
  {
    "overviews_promotion": "なにも伝わってこない表紙",
    "overviews_id": 2,
    "booth_url": "",←空になっている
    "book_title": "",
    "book_info_url": "http://amzn.asia/d/be7bGtk",
    "book_info_title": "商業本",
    "book_info_id": "2",
    "book_id": "",
    "book_description": ""
  },
  {
    "overviews_promotion": "布教本のはずなのにまさかの省略記法を使っている",
    "overviews_id": 3,
    "booth_url": "",
    "book_title": "",
    "book_info_url": "",
    "book_info_title": "",
    "book_info_id": "",
    "book_id": "",
    "book_description": ""
  }
  // これ以降省略
```



```json
[
    {
        "book_description": "Elasticsearch社のプロダクトであるElasticsearch・Logstash・Kibanaを用いてログを分析してみようという本です。各ミドルウェアのインストールからKibanaでグラフを描画するまでの道のりを一通り網羅しつつまとめました。インプレスR＆Dさんから商業化もされています。",
        "book_id": 1,
        "book_info": [
          {
            "book_info_id": 1,
            "book_info_title": "技術書典2 もふもふちゃんの戦い履歴",
            "book_info_url": "http://rimarimadan.hatenablog.com/entry/2017/04/12/技術書典2_もふもふちゃんの戦い履歴"
          },
          {
            "book_info_id": 2,
            "book_info_title": "商業本",
            "book_info_url": "http://amzn.asia/d/be7bGtk",
          }
        ],
        "book_title": "ログと情報をレッツ・ラ・まぜまぜ！～ELK Stack で作るBI環境～",
        "booth_url": "https://booth.pm/ja/items/490460",
        "overviews": [
          {
            "overviews_id": 1,
            "overviews_promotion": "ふざけたタイトル"
        },
        {
          "overviews_id": 2,
          "overviews_promotion": "なにも伝わってこない表紙"
        },
        {
          "overviews_id": 3,
          "overviews_promotion": "布教本のはずなのにまさかの省略記法を使っている"
        }
        ]
    },
    {
        "book_description": "『ログと情報をレッツ・ラ・まぜまぜ！～ELK Stack で作るBI環境～』の続編に当たる本です。今回はデータを収集・加工する工程に重点をおいて解説しています。Logstashを使ってALBのログを取得、filterプラグインを用いてデータを扱いやすい形に加工する方法について紹介しています。加えて、LogstashとFluentdの設定を見比べつつ、結局どちらを選択すればいいんだろう？ということも考えています。こちらは『Introduction Elastic Stack6』と合わせて商業化されました。",
        "book_id": 2,
        "book_info": [
        {
          "book_info_id": 1,
          "book_info_title": "技術書典3 告知",
          "book_info_url": "http://rimarimadan.hatenablog.com/entry/2017/10/17/技術書典3_告知",
        },
        {
          "book_info_id": 2,
          "book_info_title": "技術書典3に参加しました",
          "book_info_url": "http://rimarimadan.hatenablog.com/entry/2017/10/23/技術書典3に参加しました",
        },
        {
          "book_info_id": 3,
          "book_info_title": "商業本",
          "book_info_url": "http://amzn.asia/d/e3iTGAa",
        }
        ],
        "book_title": "データを加工する技術",
        "booth_url": "https://mofu-mofu.booth.pm/items/666604",
        "overviews": [
          {
            "overviews_id": 1,
            "overviews_promotion": "本編と全く関係ないペンギンの表紙"
          },
          {
          "overviews_id": 2,
          "overviews_promotion": "章が変わったときにページを詰める方法がわからず、むりやりペンギンの写真を詰め込んだ根性"
          },
          {
            "overviews_id": 3,
            "overviews_promotion": "魔法使いプリキュア！に似ているというだけで採用された表紙のフォント"
          }
        ]
    },
    {
        "book_description": "Elastic Stackがバージョン6になったので、バージョン6で追加された機能やElasticsearch APIの操作方法の知見を寄せ集めました。導入するだけでKibanaのBI画面作成までを実施してくれるFilebeat Modulesの使用例はバージョン6の目玉機能なのでは、と個人的に考えています。もちろんKibanaのUIがアップデートされたため、それに追従してKibanaを操作しつつデータを可視化する知見も入れ込みました。こちらは『データを加工する技術』と合わせて商業化されています。",
        "book_id": 3,
        "book_info": [
          {
            "book_info_id": 1,
            "book_info_title": "技術書典4にサークル参加します！",
            "book_info_url": "http://rimarimadan.hatenablog.com/entry/2018/04/17/技術書典4にサークル参加します！",
          },
          {
            "book_info_id": 2,
            "book_info_title": "技術書典4に参加しました。",
            "book_info_url": "http://rimarimadan.hatenablog.com/entry/2018/04/23/技術書典4に参加しました%E3%80%82",
          },
          {
            "book_info_id": 3,
            "book_info_title": "商業本",
            "book_info_url": "http://amzn.asia/d/e3iTGAa",
          }
        ],
        "book_title": "Introduction Elastic Stack6",
        "booth_url": "https://mofu-mofu.booth.pm/items/965753",
        "overviews":[
          {
            "overviews_id": 1,
            "overviews_promotion": "考えるのが面倒臭くて適当に作った表紙"
          },
          {
            "overviews_id": 2,
            "overviews_promotion": "この本のリポジトリのgit logを取得しているので駆け込み原稿であることがもろバレである"    
          },
          {
            "overviews_id": 3,
            "overviews_promotion": "ちなみに（ユーザー名はマスキングして）そのまま商業化されている"
          }
        ]
    },
    {
        "book_description": "新卒4年目時点で感じた「こういう風に学べばよかった」「こういう風に質問すればよかった/されると嬉しい」「こんな人にはなりたくないなあ」「どうやって調べたらいいんだろう」的なエモポエムに対する感想＋もふもふちゃんのエモポエム集です。自分への戒め＋1年目で知ってたらもっと変わったかもな〜〜〜他の人にも教えたろ！みたいないい迷惑の知見を布教するつもりで書いた本その1です。",
        "book_id": "4",
        "book_info":[
          {
            "book_info_id": 1,
            "book_info_title": "技術書典4にサークル参加します！",
            "book_info_url": "http://rimarimadan.hatenablog.com/entry/2018/04/17/技術書典4にサークル参加します！",
          },
          {
            "book_info_id": 2,
            "book_info_title": "技術書典4に参加しました。",
            "book_info_url": "http://rimarimadan.hatenablog.com/entry/2018/04/23/技術書典4に参加しました%E3%80%82",
          },
          {
            "book_info_id": 3,
            "book_info_title": "とらのあな通販",
            "book_info_url": "https://ec.toranoana.jp/tora_r/ec/item/040030674964",
          }
        ],
        "book_title": "ひよこエンジニアに送るお仕事サバイバルガイド",
        "booth_url": "https://mofu-mofu.booth.pm/items/965770",
        "overviews":[
          {
            "overviews_id": 1,
            "overviews_promotion": "遊び紙の白い部分をこすると（無駄に）レモンのにおいがする。いいにおい。"
          },
          {
          "overviews_id": 2,
          "overviews_promotion": "KUSOみたいな人がいて「きぃ〜〜〜〜〜っ！」となったので書いた"
        },
        {
          "overviews_id": 3,
          "overviews_promotion": "なぜか「あなたひよこちゃうやろ！」って人の元に多くの本が旅立っていった"
        }
        ]
    },
    {
        "book_description": "『ひよこエンジニアに送るお仕事サバイバルガイド』では書けなかったようなブラックなネタ（温めておいた）だけを集めて書いたのですが、このときはもう転職していたのですっかりやる気がなくなって…はないです。もふもふちゃんが仕事で出会ってやだったな〜〜〜と思った人のアンチパターン事例を集めました。自分が仲間入りしないように頑張ります。",
        "book_id": "5",
        "book_info":[
          {
            "book_info_id": 1,
            "book_info_title": "コミックマーケット94に出場します",
            "book_info_url": "http://rimarimadan.hatenablog.com/entry/2018/07/17/コミックマーケット94に出場します",
          }
        ],
        "book_title": "ひよこエンジニアに送るサバイバルガイド 〜深淵の闇を見た大人編〜",
        "booth_url": "https://mofu-mofu.booth.pm/items/965778",
        "overviews":[
          {
            "overviews_id": 1,
            "overviews_promotion": "仕事で全然技術的なことができず、本のネタが作れないのにコミケにでることになった"
          },
          {
            "overviews_id": 2,
            "overviews_promotion": "新刊がないのは寂しいので「このKUSSSSSSSO〜〜〜〜〜」という気持ちを起こしてみた"
          },
          {
            "overviews_id": 3,
            "overviews_promotion": "本が薄くてぺにゃってなる。ペニャペニャの本です"
          }
        ]
    },
    {
        "book_description": "Webアプリケーション関連の用語、色々あって毎回調べ直すのめんどくさ…そうだ用語集を作ろう！と思って原稿を始めました。進めるうちに「用語集だと結局読まない気がする」と思い始め、問題を考えて回答を作れば理解も深まるのでは…という結論に至りました。理解が深まったか…はちょっと自信がありませんが、学校の定期テストを考えている先生はほんと大変だなという気持ちになりました。通販は大好きなとらのあなさんにお願いしました。個人的にもお世話になっているので、とらのあな専売シールをつけてもらいたかったんです。",
        "book_id": "6",
        "book_info":[
          {
            "book_info_id": 1,
            "book_info_title": "技術書典5に参加できるか怪しかったけど参加できた話",
            "book_info_url": "http://rimarimadan.hatenablog.com/entry/2018/10/10/技術書典5に参加できるか怪しかったけど参加で",
          },
          {
            "book_info_id": 2,
            "book_info_title": "技術書典のサークルページ",
            "book_info_url": "https://techbookfest.org/event/tbf05/circle/41000006",
    
          }
        ],
        "book_title": "第1回 Webエンジニア基礎学力試験 問題と解説",
        "booth_url": "https://ec.toranoana.jp/tora_r/ec/item/040030674965",
        "overviews":[
          {
            "overviews_id": 1,
            "overviews_promotion": "このためだけに高校数学の参考書を買った。しかも解説がわからなかったのでわかる人に教えてもらった。高校生かな？"
          },
          {
          "overviews_id": 2,
          "overviews_promotion": "過去問題集を参考に表紙を作った。見開きで作ったら閉じ方向を間違えて作り直しになった。"
        },
        {
          "overviews_id": 3,
          "overviews_promotion" : "中身よりも組版や表紙を頑張った気がする。どこを目指しているんだ…"
        }
      ]
    }
] 
```

コミットログには

> に・ど・ど・やりたくない！

と書いてありました。全くもって同感です。間違いや抜け漏れがあるとは思いましたが、不具合を見つけたときに修正する方が効率が良いと感じたため一旦これにてJSONの作成は終了としました。

https://github.com/MofuMofu2/portfolio-vue/pull/26

## #19 JSONからデータを取得して表示できるようにする

今回のアプリケーションで一番難しかった部分です。動作するまでに6日もかかりました。何が難しかったのかというと、

- ローカルの配列をどうやって読み込めば良いのか？
- JSONから特定のカラムの情報を抜くにはどうすればいいのか？
- JSONに1：Nのデータがあるときはどうすればいいのか？

この実装方法がわからずに詰まってしまいました。

### JSONからデータを取得する方法を探す

はじめに考えたことは、親コンポーネントの``App.vue``で独自のidを持ち、それの値に応じて子コンポーネントがJSONからデータを取得する、というものでした。

<!-- 図をかく -->

このようにすれば、「今どの同人誌の情報を表示すれば良いか？」を算出するためのロジックを``App.vue``へ１回書くだけですみます。同じようなロジックを6回近く記載するのは大変面倒ですし、データの不整合が起きてしまうかもしれません。一括で管理しておきたいなと思いました。Vue.jsには``prop``という仕組みがあります。これを利用して親コンポーネントから子コンポーネントへデータを渡すことができます。子コンポーネントから親コンポーネントへデータを渡す場合、``emit``という仕組みを利用します。今回は``prop``を利用すれば目的が達成できそうだ、と思いました。

<!-- 図を書く -->

いきなり``prop``の処理を実装するのは無謀だと思いました。Vue.jsでJSONからデータを取得するサンプルコードは公式ドキュメントに載っていませんし、他の実装例ではアプリケーション外部のサーバーからJSONをHTTPの通信（GET）で取得する方法が多かったためです。具体的には``axios``というライブラリを用いてHTTP通信を行う実装が多く、ローカルからJSONを取得する事例はあまりありませんでした。こうなると自分で考える必要があります。一度に1個のことしか考えることができない、とわかっていたので「はじめはJSONから1つのデータを取得」「idの切り替えでデータを切り替えるのはその後」としました。今思うと正解だったと思います。

実装のヒントはないか、とGitHubのissueやVue.jsのフォーラム、Stack Overflowなどの記事を読んでみました。すると、大きく分けて2つの事象で詰まってしまう人が多い、ということがわかりました。

1. assetsフォルダにあるデータはimportで取得する。このときにパスの指定がうまくいかずにハマる人が多い。
2. ローカル以外（外部URL・APIサーバーから…など）はaxiosを利用する。ローカルのデータをaxiosで取ろうとしてうまくいかない！という内容の質問箱には「いやローカルならimportで取れますけど」と言われていることが多かった。

``1.``は画像のインポート時に利用した``require``を利用すれば回避できそうだ、と考えました。問題は``2.``です。``axios``を利用する必要がないことはわかりました。では、一体どうやってローカルのJSONを取得すれば良いのでしょう？じゃあ方法を書いてくれ〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜っ！！！！！という憤りを感じてブラウザバックする、これの繰り返しでした。

調べに調べた結果、一番参考になった実装はサークル``べころもち工房``の```べこ``さんの実装でした。技術書典3で「このもちみたいなキャラクターがかわゆすな〜」と思って購入した同人誌だったのですが、こんなところで役に立つとは…！！！技術書典の出会いと自分の目利き力に感謝しました。本の名前は『Vue.jsでポートフォリオサイト制作記』です。

べこさんのポートフォリオでは、自分の情報を[JSONで保持](https://github.com/becolomochi/shirokuma/blob/master/static/data.json)（``https://github.com/becolomochi/shirokuma/blob/master/static/data.json``）していました。同人誌の内容を参考にしつつ、このような実装を書いてみました。

``import``でJSONデータをインポートし、``data``部分でデータを割り当てれば良いのでは？と考えたからです。

```JavaScript
 <template>
   <div class="description-list">
     <div class="overview">
       <h3>KUSO POINT</h3>
       <ul>
         <li v-for="(item, key) in overviews" v-bind:key="item.id">
           {{ item.promotion }}
        </li>
      </ul>
    </div>
  </div>
</template>

<script>
import promotionData from '../assets/book-data.json'
export default {
  name: 'descriptionList',
  data() {
    return {
      promotionData: promotionData,
      overviews: [
        { promotion: '' },
        { promotion: '' },
        { promotion: '' }
      ]
    }
  }
}
</script>
``` 

しかし、これではHTMLタグへデータが割り当たっていません。``v-bind``で``promotionData``を利用していないので当たり前なのですが、このときは全く気付きませんでした。そこで、importしたJSONの割り当てがどこまでできているのかデバッグすることにしました。
まず、何かしらの処理を行って算出したデータを割り当てる場合は``computed``を利用するらしい、という情報を目にしました。そこでとりあえず``computed``を使ってみることにしました。次に、``console.log()``を使ってChromeの``Console``タブに文字の出力をするようにしました。こうすればどこまでデータがきているわかると思ったからです。出力する情報は、JSONの情報が全てが入っているはずの``bookData``と、``bookData``の１番目に入っている``overviews``内の``overviews_promotion``としました。JSONの情報は配列になっているのでは？と推測したためです。

```JavaScript
    <div class="overview">
      <h3>KUSO POINT</h3>
      <ul>
-        <li v-for="(item, key) in overviews" v-bind:key="item.id">
-          {{ item.promotion }}
+        <li v-for="(item, keys) in promotionData " v-bind:key="item.id">
+          {{ item.overviews_promotion }}
        </li>
      </ul>
    </div>
  </div>
</template>
 <script>
- import promotionData from '../assets/book-data.json'
+ import bookData from '../assets/book-data.json'
export default {
  name: 'descriptionList',
  data() {
    return {
-       promotionData: promotionData,
-       overviews: [
-         { promotion: '' },
-        { promotion: '' },
-         { promotion: '' }
-       ]
+       bookData: bookData,
+     }
+   },
+   computed: {
+     promotionData: function() {
+       console.log(bookData);
+       console.log(bookData[0].overviews.overviews_promotion);
+       return bookData[0].overviews.overviews_promotion;
     }
  }
}
```

![Chromeのデバッグ結果](C95-vue-and-nuxt/images/chapter5/#19_finish.png)

デバッグ画面で確認してみると、予想通り``bookData``＝JSONの情報は配列として取得されています。配列の中に1つの本に対して１つのオブジェクトが入っているようです。結果が出力されているということは、ここまでは意図した動作になっているのだ、と推測できます。問題は次です。
2つ目の出力が``undefined``となっています。直訳すると``未定義``という意味ですが、何も定義がない状態ではありません。``undefined``という値は``undefined``という初期の値がセットされており、上書きもできない固定の値です。
FireFoxを開発しているMozillaのWeb技術解説サイト[MDN web docs](https://developer.mozilla.org/ja/docs/Web/JavaScript/Reference/Global_Objects/undefined)（``https://developer.mozilla.org/ja/docs/Web/JavaScript/Reference/Global_Objects/undefined``）では

> - 配列や変数の値が代入されていない
> - まだ値が代入されていない
> - 値を``return``しない関数

このような場合は``undefined``が返却される、と解説されています。

この情報から、``bookData[0].overviews.overviews_promotion``というデータの取得方法は正しくないという推理をすることができます。配列に``bookData[0].overviews.overviews_promotion``が示すデータは存在しないから``undefined``になっていると考えられるためです。2つの``console.log``で``undefined``が返却されているのであれば、JSONのデータが取得できていないと考えられるのですが、今回はそうではありません。
よって、配列からデータを取得する部分がおかしいのだと推理しました。そこで、いくつか思いついた方法を試して結果を比較してみることにしました。

```JavaScript
  computed: {
    console.log(bookData);
    const dataList = bookData;
    console.log(dataList[0]);
+     console.log(dataList[0].overviews);
    console.log(bookData[0].overviews.overviews_promotion);
    return bookData[0].overviews.overviews_promotion;
  }
```

この方法だと、overviews内に入っているデータの配列を全て取得することができました。しかし、今回は``overviews_promotion``に紐づく情報を1つずつ取得したいのです。

ここで、配列の値の取得方法を再度確認することにしました。配列から値を取得する場合、

```JavaScript
配列[インデックス番号]
```

と記載します。これは``dataList[0]``で同人誌の情報が入った配列の1番目の値が取得できています。配列の値にまた配列が入っている場合、そこにアクセスするためにも配列のインデックスを指定する必要があるのでは？と考えました。

<!-- これは図を書く -->

今の実装では配列Aのデータにアクセスするときはインデックスを指定していますが、配列Aの中の配列Bにアクセスするときはインデックスを指定していません。結果、配列Bのどのインデックスの情報を取得すれば良いのか判断できず、``undefined``になっているのでは？と考えました。

そこで、配列Bに当たる``overviews``にもインデックスを明示的に指定するような実装を書いてみました。

```JavaScript
  computed: {
    console.log(bookData);
    const dataList = bookData;
    console.log(dataList[0]);
    console.log(dataList[0].overviews);
    console.log(bookData[0].overviews.overviews_promotion);
+   console.log(bookData[0].overviews[0].overviews_promotion);
    return bookData[0].overviews.overviews_promotion;
  }
```

結果として、コンソールに配列内にある``overviews``の値を出力することができました。

### JSONからデータを繰り返し取得する

配列のインデックスを繰り返し処理で変化させることができれば、JSONの子項目を1回ずつ``v-bind``したリストを作成できるはずです。そこで、``ForEach``文を利用して処理を作ってみることにしました。``for...of文``や``for文``でも同じような処理が作れると思います。利用用途に合わせて選べるようになりたいですね。

```JavaScript
  computed: {
    promotionData: function() {
-       console.log(bookData);
-       const dataList = bookData[0].overviews;
-       console.log(bookData[0].overviews[0].overviews_promotion);
-       return bookData[0].overviews.overviews_promotion;
+       // 繰り返し処理でoverviews_promotionの値だけ抜いてreturnする
+       const dataList = bookData[0].overviews.forEach((array_element, index, array) => {
+         console.log(array_element);
+       });
    }
  }
}
```

配列を繰り返し取得することができたので、今度は取得した配列の値だけを代入した配列を新規に作成する必要があります。``promotionData``関数で返却する値として利用するためです。今回は``push``メソッドを利用して、配列から取得した値を変数に入れ、その変数を返却する値である配列に追加する処理を記載しました。最初は配列から値を取得する部分とは``push``の処理を一緒に実施しようと考えていたのですが、``undefined``になってしまったので修正しました。また、forEachを書ける自信がなかったので``for文``で記載することにしました。

```JavaScript
  computed: {
    promotionData: function() {
-       // 繰り返し処理でoverviews_promotionの値だけ抜いてreturnする
-       const dataList = bookData[0].overviews.forEach((array_element, index, array) => {
-         console.log(array_element);
-       });
      // ここから下を追記
      // 繰り返し処理でoverviews_promotionの値を全部抜いて新しい配列を作り、returnする
      const arrayLength = bookData[0].overviews.length;
      const arrayData = bookData[0].overviews
      console.log(arrayLength);
      console.log(arrayData);
      console.log(arrayData[0].overviews_promotion);
      const promotions = []
      for (let count = 0; count < arrayLength; count++) {
        const data = arrayData[count].overviews_promotion;
        promotions.push(data);
      }
      console.log(promotions);
      return promotions;
    }
  }
}
```

### りまりま団の同人誌リスト

- https://docs.google.com/spreadsheets/d/16NGDz_8Xl4hAzjCPNHjx5pVN8cxNFOsoPngeLrF633A/edit?usp=sharing

### SQLIFY（CSVをJSONに変換する）

- https://sqlify.io/convert

### JSONからデータを取得する

#### Why i can't filter local json file in this case (vuejs2)

- https://stackoverflow.com/questions/52239094/why-i-cant-filter-local-json-file-in-this-case-vuejs2

#### VueJS - Reading data from local json file into vis.js timeline

- https://stackoverflow.com/questions/45425448/vuejs-reading-data-from-local-json-file-into-vis-js-timeline

#### Vue.jsでポートフォリオサイト制作記

参考にしたページはp28と29です。

- https://booth.pm/ja/items/667217

#### べころもち工房のべこさんのリポジトリ（JSONからデータを取得する部分）

- https://github.com/becolomochi/shirokuma/blob/master/static/data.json

### JavaScriptで配列やオブジェクトの操作方法を調べる

#### js-primer オブジェクト

MDNと一緒にいつも参考にさせていただいてます。

- https://jsprimer.net/basic/object/

#### js-primer 配列

- https://jsprimer.net/basic/array/

#### undefined

- https://developer.mozilla.org/ja/docs/Web/JavaScript/Reference/Global_Objects/undefined