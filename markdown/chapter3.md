# #1 ヘッダーの作成〜#2 フッターの作成

先に見た目を作って色々調整しよう！わかりやすいから！という方針で進めました。これは大正解だったと思います。なぜなら、見た目ができていると安心感がありますし、進捗していることが目で見てわかるからです。

個人開発はモチベーションを保つのが難しいので、何かしら「進捗している」感を演出する仕掛けを作っておいた方が良いです。今回は、1コンポーネントごとにissueをたてることにしました。さらに、開発時に出た不具合や改善点もissueを記載することにしました。issueが閉じられると何かしらの進捗を感じられ、かつGitHubのContributionにも反映されるため一石二鳥です。

## #1 ヘッダーの作成

まずはじめに、ヘッダーを作成します。今回のワイヤーフレームの中では画像の四角で囲ったところをヘッダーとしました。

![ヘッダー](../images/chapter3/header.png)

### Vue.jsの基本的な書き方でヘッダー要素を作成する

### ヘッダー要素に割り当てる文字をdataに記載する

![ヘッダー作成後](../images/chapter3/1_header.png)

### CSSで見た目を整える



最終的な見た目はこのようになりました。ヘッダーを作るという目的は達成できたので、ブランチをGitHubにpushし、masterブランチへmergeしました。

![最終的なヘッダー](../images/chapter3/1_finish.png)

## #2 フッターの作成


今度はフッターを作成します。ワイヤーフレームではフッターに色をつけていたのですが、試しに実装してみると目がチカチカしてしまいました。気分も悪くなりそう（酔ったみたいな感じ）なので、これはボツにしました。

何を記載すれば良いか迷ったので、ほかのポートフォリオサイトにどんな要素が載っているのか調べてみることにしました。多かった項目は次の3つです。

- SNSのアカウント情報
- （技術に関するものの場合）GitHubのアカウント情報
- コピーライト表記

SNSのアカウント情報はTwitterとFacebookの採用率が飛び抜けていました。やはり盛り上がっていて拡散力があるのでしょう。次点でInstagramが多かったです。Webサイト巡りをしていて感じたのは、情報が多いとゴテゴテして見づらいということと、文字よりアイコンの方がわかりやすいということです。
色々書いてあったとしても、結局読まれなければと意味がないですよね。それに文字がぎっちり詰まったサイトは読みづらいです。加えてアクセスした人のPCモニターサイズが大きいとは限りません。小さい窓にたくさん文字がある状態では、とてもじゃないけどサイトをじっくり見ようという気持ちにはなれません。

結果、次の3つをフッターに記載しようと考えました。

- SNSのアカウント（Twitter）情報
- KUSOアプリケーションのGitHubリポジトリ
- コピーライト表記

最初はコピーライト表記のことなど頭になかったのですが、これがないと自分のWebサイト内の文章や画像などを転載されてしまったときに「表記ないんだからいいじゃん」と言われてしまいます。別に転載するほどの情報ではない…とは思ったのですが、念の為入れておくことにしました。後から後悔しても遅いですからね。

### コピーライト表記の書き方を調べる

書き方がわからないので、Webサイトを回遊しているときに見かけたコピーライト表記を真似しようと思いました。最初はWebサイト表記で一番多かった、``Copyright ©️ 2018-2018 りまりま団 All Rights Reserved.`` のように記載すれば良いのかと思ってました。しかし、これはどうやら正確な書き方ではないようでした。

まず、``©︎``は``コピーライト``という意味のため、``Copyright``は不要となります。コピーライトコピーライトと記載してあるのと意味が同じだからです。無駄な表現は削っておきたいですよね。次に年号の表記方法ですが、絶対に必要なのは**そのWebサイトを立てた年＝発刊年**だけであって、``-2018``の部分はWebサイトが更新された年号を表しているだけで表記が必須ではありませんでした。
最後に``All Rights Reserved.``の部分はブエノスアイレス条約という1910年に成立された著作権を守るための条約内で規定されている文言であり、日本はブエノスアイレス条約に加盟していないため、記載しても著作権の保護を受けることはできません。日本に国籍がある私が作ったKUSOポートフォリオは、この文言で著作権の保護を受けることはできないということです。つまり``All Rights Reserved``表記は不要となります。

これらの情報を総合した結果、最終的なコピーライト表記は

> ©︎ 2018 りまりま団

となりました。

表記方法を決めたので、アプリケーションにこれを反映します。DOMにデータを割り当てして表示する・またはHTMLに直接記載する2パターンで描画することができます。

### SVGアイコンを利用した外部リンクを作成する

Webサイト外のURLに接続したい場合、HTMLでは``a``タグを利用してURLを表示することができます。これではURLが文字で表示されてしまいます。``a``タグの中に文字を入れればURLにジャンプできる文字を作成することはできます。しかし、TwitterやGitHubのようにアイコンが有名なコンテンツへのリンクであれば、アイコン自体をURLリンクにしたいですよね。わかりやすいですし、情報量が少ない分見栄えも良くなります。

そこで、TwitterとGitHubの画像アイコンを入手し、それにURLを割り当てて外部リンクアイコンにしようと考えました。

### アイコン画像を入手する

まずはじめに、アイコン画像を入手します。

GitHubは[GitHub Logos and Usage](https://github.com/logos)（``https://github.com/logos``）から公式のアイコンをダウンロードすることができます。ダウンロードページには、許可されていること・されていないことが記載されています。ルールを簡単にまとめると、

- ロゴやOctocatを自分のアプリケーションのアイコンとして利用してはいけない
- アイコンの画像を改変してはいけない
- 自分の絵にロゴやOctocatを使ってはいけない
- GitHubの許可なくロゴやOctocatの何かを販売してはいけない
- アイコンの色・書体の変更は禁止

となっています。つまり、GitHubへページ遷移する・GitHubに関する記事の投稿時にアイコンを利用する分にはいいけれど、それ以外の用途で利用するのは禁止ということです。

GitHubのアイコンをダウンロードすると、PNG画像とaiファイル・epsファイルが圧縮された物を手に入れることができます。今回はSVGを利用したいと思ったので、aiファイルからIllustratorを利用してSVGファイルを作成しました。寸法の変更は禁止されているため、aiファイルを開いた後、そのままSVG画像を書き出しました。
Vue.jsアプリケーションでアイコンを利用したいので、``src/assets``ディレクトリの下にアイコンを配置します。

Twitterのアイコンは[Twitterのブランドリソース](https://about.twitter.com/ja/company/brand-resources.html)（``https://about.twitter.com/ja/company/brand-resources.html``）からダウンロードします。TwitterはSVGファイルが直接配布されているため、これを直接利用します。Twitterの場合、アイコンをダウンロードするとガイドラインを記載したPDFもダウンロードできます。もちろんTwitterのサイト内に記載があります。こちらはわざわざ絵で解説が書いてあります。利用前によく確認します。と言っても、ルールはほとんどGitHubと一緒です。唯一異なる点は、Twitterの古いアイコンを利用してはいけないという点です。利用する気にはなりませんが…。
こちらもサイズの変更などはせず、ダウンロードしたものをそのまま``src/assets``ディレクトリに配置します。

### アイコン画像をVue.jsで読み込む

Vue.jsでローカルの画像を表示する場合、``a``タグ内に``img``タグを配置し、画像URLのデータを``v-bind``で割り当てます。``v-bind``に割り当てる情報は変化する予定がないため、#1のヘッダー作成時と同じく``data``を利用することにしました。
今までの知識を総合すると、次のように記載すれば動くはずです。

```JavaScript
<template>
  <div class="page-footer">
    <div class="icons">
      <a href="https://twitter.com/froakie0021" target="blank">\r
      <img :src="twitter_src" alt="twitter"></a>
      <a href="https://github.com/MofuMofu2/portfolio-vue" target="blank">\r
      <img :src="github_src" alt="github"></a>
    </div>
    <div class="copyright">
      <p>©︎ 2018 りまりま団</p>
    </div>
  </div>
  
</template>

<script>
export default {
  name: 'pageFooter',
  data() {
    return {
      twitter_src: \r
      "../assets/footer/Twitter_Social_Icon_Rounded_Square_Color.svg",
      github_src: \r
      "../assets/footer/GitHub-Mark.svg"
    }
  }
}
</script>
```

しかし、これはうまくいきません。画像が表示されないのです。回避方法を先に言ってしまうと、``src``のバインドをするときに``require``を利用すれば良いです。実際に試してみましょう。
GitHubアイコンのパスには``require``をつけ、Twitterのアイコンのパスはそのまま``data``に記載します。

```JavaScript
<script>
export default {
  name: 'pageFooter',
  data() {
    return {
      twitter_src: \r
      "../assets/footer/Twitter_Social_Icon_Rounded_Square_Color.svg",
      github_src: \r
      require("../assets/footer/GitHub-Mark.svg")
    }
  }
}
</script>
```

Chromeのデバッグツールを使って、どんなデータがimgタグにバインドされているか確認します。変だなと思ったらすぐデバッグツール。これはテストに出ます。
まず、正しく表示されるはずの``github_src``にどんなデータが割り当たっているか確認しました。

```json
github_src:\r
"/img/GitHub-Mark.4336b50b.svg"
```

VueアプリケーションをVue CLIで構築する際、アプリケーションの構成ディレクトリ内のファイルを再構成して``dist``ディレクトリへ再配置します。このとき、``assets``ディレクトリ内の画像も一緒に``dist``ディレクトリに再配置されます。``require``を利用することで、再配置後の画像ファイルパスを割り当てることができるため、画像が表示できるのです。
では、Twitter画像のファイルパスはどうなっているのでしょうか？

```json
twitter_src:\r
"../assets/Twitter_Social_Icon_Rounded_Square_Color.svg"
```

こちらはファイルパスがそのまま割り当てされてしまっています。Vueアプリケーションが参照するのは``dist``ディレクトリの中ですから、これでは画像フォルダを参照することはできません。その結果、画面は正しく描画されているけれど画像がない状態になってしまうのです。
``require``を利用することで、画像ファイルのパスを正しく描画することができる、というわけですね。初めてだとかなり詰まるポイントだと思います。

### CSSを追加する

画像ファイルパスの記載方法がわかったので、CSSも追加した最終形を作成しました。

```JavaScript
<template>
  <div class="page-footer">
    <div class="icons">
      <a href="https://twitter.com/froakie0021" target="blank">\r
      <img :src="twitter_src" alt="twitter"></a>
      <a href="https://github.com/MofuMofu2/portfolio-vue" target="blank">\r
      <img :src="github_src" alt="github"></a>
    </div>
    <div class="copyright">
      <p>©︎ 2018 りまりま団</p>
    </div>
  </div>
  
</template>

<script>
export default {
  name: 'pageFooter',
  data () {
    return {
      twitter_src: require("../assets/Twitter_Social_Icon_Rounded_Square_Color.svg"),
      github_src: require("../assets/GitHub-Mark.svg")
    }
  }
}
</script>

<style scoped>
  .page-footer {
    background-color: #E5E400;
    height: 100px;
  }
  .icons {
    margin-right: 30px;
    text-align: right;
  }
  .icons img {
    height: 30px;
    margin: 10px;
    width: 30px;
  }
  .page-footer p {
    color: #32BDED;
    text-align: center;
  }

</style>
```

ブラウザで確認すると、このようになっています。

![フッター作成後](../images/chapter3/2_finish.png)

まだ見た目はしょぼいのですが、目的は達成できたのでmasterブランチにMergeしました。

## 参考URL

### Issues

- https://github.com/MofuMofu2/portfolio-vue/issues/1
- https://github.com/MofuMofu2/portfolio-vue/issues/2

### Pull Requests

- https://github.com/MofuMofu2/portfolio-vue/pull/11
- https://github.com/MofuMofu2/portfolio-vue/pull/12

### Vue.jsでimgタグのsrcを利用する

- Vue.jsでimgタグのsrcをバインドさせる際の注意点 
  - http://tk2000ex.blogspot.com/2017/11/vue.html
- img src data binding?
  - コメントの前半は内容が古くなっているため注意が必要です。``require``が出てきている箇所あたりを参考にしましょう。
  - https://github.com/vuejs/Discussion/issues/202#issuecomment-330567354