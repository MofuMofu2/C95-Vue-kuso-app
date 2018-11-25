# #4〜#8 v-bindで同人誌の情報を表示するコンポーネント群を作成

今までは固定の情報をv-bindに割り当てて表示するコンポーネントを作成しました。しかし、このポートフォリオの目標は今まで発刊した同人誌の情報を一つのサイトにまとめることです。つまり、将来的にはv-bindのデータを動的に切り替える必要があります。
いきなり実装するのは難しそうなので、まずはHTML・CSS・基本的なv-bindの処理だけを先に記載することにしました。実装後に感じたことなのですが、見た目を作る作業と処理を書く作業は別々に行った方がいいなと思います。不具合が起きてしまったときに、見た目と処理、どちらの実装が悪いのか判断するのが難しいためです。個人的には見た目を先に作り、処理を書いた方が進めやすいなと感じました。最終形が早く見える方がモチベーションを高く保つことができるためです。

## #4 KUSOタイトル表示コンポーネントの作成

https://github.com/MofuMofu2/portfolio-vue/issues/4

まず、コンポーネント用のファイルを作成し、App.vueへimportします。アプリケーションのmainコンテンツもKUSO感を出したいと思ったので、``<style>``タグに背景色（すごい色の黄色）を追加しました。また、ヘッダー・フッターとの距離が詰まって見えたので``main``タグには``margin``を設定しました。これで上下左右の間隔を空けることができます。

### App.vueにKUSOタイトル表示コンポーネントを読み込む

```JavaScript
<template>
  <div id="app">
    <title-header></title-header>←ここを追加
    <div class="main">
      <book-title></book-title>
    </div>
    <page-footer></page-footer>
  </div>
</template>

<script>
import 'normalize.css'
import titleHeader from './components/Header.vue'
import bookTitle from './components/BookTitle.vue'←ここを追加
import pageFooter from './components/Footer.vue'
 export default {
  name: 'app',
  components: {
    'title-header': titleHeader,
    'book-title': bookTitle,←ここを追加
    'page-footer': pageFooter
  }
}
</script>

<style>
   #app {
    background-color: #E5E400;
  }
  .main {
    margin: 20px;
  }
</style>
```

### 同人誌を表示するためのコンポーネントを作成する

今度は同人誌を表示するためのコンポーネントを作成します。実際に開発するときは、

1. ファイル作成
2. ``<style>``などのテンプレートを記載
3. App.vueにインポートする処理を記載
4. コンポーネントの中身を記載

という順番で進めています。こうすればブラウザでコンポーネントの内容を確認しながら進めることができますし、インポートし忘れを防ぐことができます。
コンポーネントの開発内容はヘッダー・フッターと特に代わりなく、特定の文字列を特定のHTMLタグにv-bindで割り当てるだけです。

```JavaScript
<template>
  <div class="book-title">
    <div class="fanbook-title">
      <fan-book v-bind:title="item">{{ item }}</fan-book>
    </div>
  </div>
</template>
 <script>
export default {
  name: 'bookTitle',
  data() {
    return {
      item: "ログと情報をレッツ・ラ・まぜまぜ！～ELK Stack で作るBI環境～"
    }
  }
}
</script>
 <style scoped>
  .book-title {
    background-color: #E5E400;
    color: #32BDED;
    font-size: 30px;
  }
  .fanbook-title {
    width: 520px;
  }
</style>
```

HTMLタグにデータを割り当てる方法は2種類あります。使い分けに迷ったので公式ドキュメントや実装例を見たところ、

- 特定の決まったデータを割り当てるのであれば``v-bind``
- テキストボックスに入力した値を画面でもすぐ表示したい、といったように双方向の描画が必要なときは``v-model``

このように使い分けるのが良さそうだ、と思いました。利用比率的には圧倒的に``v-bind``の方が多いな、という印象です。双方向描画は処理が重たくなりそうなのでできる限り使わないようにしているのかな、と思いました。
実装後はこのようになりました。タイトルからもKUSO感が溢れ出ていて絶好調です。

![KUSOタイトル表示コンポーネント実装後](C95-vue-and-nuxt/images/chapter4/#4_finish.png)

https://github.com/MofuMofu2/portfolio-vue/pull/13

## #3 表紙を表示するコンポーネントを作成する

https://github.com/MofuMofu2/portfolio-vue/issues/3

今度は表紙を表示するコンポーネントを作成します。注意点はフッター作成時と同じく、画像フォルダのパスを``require``を使って割り当てする部分です。同じような処理は一気にまとめて作ってしまった方が忘れなくて良い気がします。これはまとめて作っていないのですが…。

### App.vueに表紙を表示するコンポーネントを読み込む

``#4``の実装と同じく、コンポーネント用のファイルを作成してApp.vueにimport処理を記載します。

```JavaScript
<template>
  <div id="app">
    <title-header></title-header>
    <div class="main">
      <book-title></book-title>
      <book-image></book-image>←ここを追加
    </div>
    <page-footer></page-footer>
  </div>
</template>
<script>
import 'normalize.css'
import titleHeader from './components/Header.vue'
import bookImage from './components/BookImage.vue'←ここを追加
import bookTitle from './components/BookTitle.vue'
import pageFooter from './components/Footer.vue'
 export default {
  name: 'app',
  components: {
    'title-header': titleHeader,
    'book-image': bookImage,←ここを追加
    'book-title': bookTitle,
    'page-footer': pageFooter
  }
}
</script>

<style>
   #app {
    background-color: #E5E400;
  }
  .main {
    margin: 20px;
  }
</style>

```

特段変わったことはありませんね。

### 同人誌の表紙を表示するコンポーネントを作成する

こちらは#2のフッターと同じように、requireを使って仮のデータを割り当てます。画面が全部できたあとで、画像の切り替えを行いたいためです。

```JavaScript
<template>
  <div class="book-image">
    <img :src="image_src" alt="bookImage">
  </div>
</template>
 <script>
export default {
  name: "bookImage",
  data() {
    return {
      image_src: require("../assets/bookImage/1.png")
    }
  }
}
</script>
 <style scoped>
  .book-image {
    height: 400px;
    margin-top: 30px;
    width: 570px;
  }
  img {
    height: 100%;
    width: 285px;
  }
</style>
```

このとき、``v-bind``を使ってHTMLにデータを割り当てる方法が間違っていたことに気づきました。コンポーネントをタグとして利用できるのは、コンポーネントをインポートしたファイルだけです。これを忘れていたので、まぐれで描画できてしまっていました。これはよくない、ということで``BookTite.vue``の表記を修正しました。

```JavaScript
<template>
  <div class="book-title">
    <div class="fanbook-title">
      <fan-book v-bind:title="item">{{ item }}</fan-book>
      <div class="fan-book" v-bind:title="item">{{ item }}</div>
    </div>
  </div>
</template>
```

最終的にこのような表示になりました。画像が一枚入るとWebサイト感があります。

![表紙を表示するコンポーネント作成後](C95-vue-and-nuxt/images/chapter4/#3_finish.png)

https://github.com/MofuMofu2/portfolio-vue/pull/14

## #5 同人誌のコメント（本文）表示コンポーネントを作成する

今度は同人誌の内容を説明するコンポーネントを作成します。

### App.vueにコメント表示コンポーネントを読み込む

何回も繰り返していると、何も見ずにコンポーネントのimportができるようになります。地味に成長を感じますね。コンポーネントのimport文はコンポーネントがアルファベット順に並ぶようにしました。VS Codeと並び順が一緒になって確認しやすいためです。

```JavaScript
   <div id="app">
     <title-header></title-header>
    <div class="main">
      <book-title></book-title>
      <book-image></book-image>
      <book-description></book-description>←これを追加
    </div>
    <page-footer></page-footer>
  </div>
 </template>
 
<script>
import 'normalize.css'
import titleHeader from './components/Header.vue'
import bookDescription from './components/BookDescription.vue'←これを追加
import bookImage from './components/BookImage.vue'
import bookTitle from './components/BookTitle.vue'
import pageFooter from './components/Footer.vue'
 
 export default {
  name: 'app',
  components: {
    'title-header': titleHeader,
    'book-description':bookDescription,←これを追加
    'book-image': bookImage,
    'book-title': bookTitle,
    'page-footer': pageFooter
   }
 }
 </script>
```

### コメント表示コンポーネントを作成する

こちらも仮のデータを``v-bind``で割り当てました。ここまでの作業を終えるのに7日かかりました。まあ平日は（これでも）働いていて、サボった日もあると考えると結構速いペースで物が作れるなあという感想を持ちました。複雑なことをしなければ、簡単に作れるっていいですよね〜。

ここから文字が登場するので、フォントサイズや行の高さをどのように設定するか迷いました。そこで、通販サイトのCSS設定などをいくつか比較し、好みの見た目がどのような設定になっているのかを調べました。今回はpixivさんのBoothを参考にしました。

```JavaScript
<template>
  <div class="book-description">
    <div class="description" v-bind:description="text">{{ text }}</div>
  </div>
</template>
 <script>
export default {
  name: 'bookDescription',
  data() {
    return {
      text: 'Elasticsearch社のプロダクトであるElasticsearch・Logstash・Kibanaを用いてログを分析してみようという本です。各ミドルウェアのインストールからKibanaでグラフを描画するまでの道のりを一通り網羅しつつまとめました。インプレスR＆Dさんから商業化もされています。'
      }
  }
}
</script>
 <style scoped>
  .book-description {
    height: 250px;
    margin-top: 20px;
    width: 570px;
  }
  .description {
    font-size: 16px;
    line-height: 26px;
  }
</style>
```

これで左側が埋まりました。今度は右側部分を作ります。

![コメント表示コンポーネント実装後](C95-vue-and-nuxt/images/chapter4/#5_finish.png)

https://github.com/MofuMofu2/portfolio-vue/pull/15