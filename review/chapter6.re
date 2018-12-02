
= 完成形に近づける


やっとJSONからデータを取得することができました。しかし、これでは他の同人誌は一生いなかったことにされてしまいます。それは許せません。カルーセルの処理を入れる/前後に進むボタンを追加するなどしてデータを切り替えできるようにします。


== #25 情報を切り替える実装を追加


まずはじめに、前後に切り替えるためのボタンを実装します。Googleが提供している@<href>{https://material.io/tools/icons/?style=baseline,Material Design-Icons}（@<tt>{https://material.io/tools/icons/?style=baseline}）から、切り替えボタン用の画像アイコンを取得しました。ライセンスはApache Licenceなので、Material Designのアイコンを利用したという情報をアプリケーションのどこかに入れればライセンス違反にはなりません。
最終的にはemojiに置き換えたため、今回は記述しませんでした。


//emlist[][JavaScript]{
<template>
   <div id="app">
     <div class="header">
       <title-header></title-header>
      <book-title></book-title>
    </div>
    <div class="main">
+       <button class="back_button" type="button">
+         <img v-bind:src="left_arrow" alt="back">
+       </button>
      <div class="left-contents">
        <book-image></book-image>
        <description-list></description-list>
       </div>
       <div class="right-contents">
         <book-description></book-description>
        <book-info></book-info>
        <booth-button></booth-button>
      </div>
+       <button class="next_button">
+         <img v-bind:src="right_arrow" alt="next">
+       </button>
    </div>
    <page-footer></page-footer>
  </div>

 <script>
 import 'normalize.css'
 import titleHeader from './components/Header.vue'
 import bookDescription from './components/BookDescription.vue'
 import bookImage from './components/BookImage.vue'
 import bookInfo from './components/BookInfo.vue'
 import bookTitle from './components/BookTitle.vue'
 import boothButton from './components/BoothButton.vue'
 import descriptionList from './components/DescriptionList.vue'
 import pageFooter from './components/Footer.vue'

 export default {
   name: 'app',
   components: {
     'title-header': titleHeader,
     'book-description':bookDescription,
     'book-image': bookImage,
     'book-info': bookInfo,
     'book-title': bookTitle,
    'booth-button': boothButton,
    'description-list': descriptionList,
    'page-footer': pageFooter
+   },
+   data() {
+     return {
+       left_arrow: require("./assets/arrow/baseline-keyboard_arrow_left-24px.svg"),
+       right_arrow: require("./assets/arrow/baseline-keyboard_arrow_right-24px.svg"),
+     }
+   }
}
</script>
//}


//image[add_button][ボタンを追加した]{
//}




見た目は見なかったことにします。


=== どの同人誌の情報を表示するか判断できるようにする


今、JSONから本のタイトル・参考URLの情報を取得するときは次のような実装となっています。


//emlist[][JavaScript]{
// JSONの配列の1番目のインデックスからbook_titleの情報を取得する
bookData[0].book_title
//}


JSONのデータ配列の1番目のインデックスからbook_titleの情報を取得する、というのが具体的な実装です。ということは、JSONのデータ配列の2番目、3番目…というように配列のインデックス名を変更できれば、本の情報を切り替えることができるはずです。どうやって切り替えればいいのでしょう？



今回はボタンを押したときに画面内全てのデータを切り替える必要があります。ということは、配列のインデックスの値を一度に切り替えなくてはいけません。そこで@<tt>{prop}の利用を検討しました。これは第5章の@<tt>{JSONからデータを取得する方法を探す}で述べたとおりです。



まずはじめに、@<tt>{App.vue}で配列のインデックスの値を定義する必要があります。その実装を行いました。ボタンがおされたときに値が変わるようにしたいため、@<tt>{v-on}を利用してクリックされたときにイベントを検知し、@<tt>{addCount}または@<tt>{removeCount}メソッドを呼び出してインデックスの値を変化させる処理を呼び出します。


//emlist[][JavaScript]{
 <template>
   <div id="app">
     <div class="header">
       <title-header></title-header>
      <book-title></book-title>
    </div>
    <div class="main">
-       <button class="back_button" type="button">
+       <button class="back_button" type="button" v-on:click="removeCount">
        <img v-bind:src="left_arrow" alt="back">
      </button>
      <div class="left-contents">
         <book-image></book-image>
         <description-list></description-list>
       </div>
       <div class="right-contents">
         <book-description></book-description>
        <book-info></book-info>
        <booth-button></booth-button>
      </div>
-       <button class="next_button">
+       <button class="next_button" v-on:click="addCount">
        <img v-bind:src="right_arrow" alt="next">
      </button>
    </div>
     <page-footer></page-footer>
   </div>
 </template>

<script>
import 'normalize.css'
import titleHeader from './components/Header.vue'
+ import bookData from './assets/book-data.json'
import bookDescription from './components/BookDescription.vue'
import bookImage from './components/BookImage.vue'
import bookInfo from './components/BookInfo.vue'
 import bookTitle from './components/BookTitle.vue'
 import boothButton from './components/BoothButton.vue'
 import descriptionList from './components/DescriptionList.vue'
 import pageFooter from './components/Footer.vue'
 export default {
  name: 'app',
+ props: ['currentBookId'],
  components: {
    'title-header': titleHeader,
    'book-description':bookDescription,
     'book-image': bookImage,
     'book-info': bookInfo,
     'book-title': bookTitle,
     'booth-button': boothButton,
     'description-list': descriptionList,
     'page-footer': pageFooter
  },
  data() {
    return {
+       bookData: bookData,
+       bookId: 0,
+       left_arrow: require("./assets/arrow/baseline-keyboard_arrow_left-24px.svg"),
-       right_arrow: require("./assets/arrow/baseline-keyboard_arrow_right-24px.svg"),
+       right_arrow: require("./assets/arrow/baseline-keyboard_arrow_right-24px.svg")
    }
+   },
+   methods: {
+     addCount() {
+       if (this.bookId !== bookData.length ) {
+         return this.bookId++;
+       } else {
+         return;
+       }
+     },
+     removeCount() {
+       if (this.bookId !== 0) {
+         return this.bookId--;
+       } else {
+         return 0;
+       }
+     }
+    }
 }
</script>

よく見ると、``prop``の使い方が間違っています。``prop:[変数名]``は子コンポーネントに記載するべきですが、ここでは親コンポーネントに記載されてしまっています。親コンポーネントでするべきことは、``v-bind``で子コンポーネントにデータを紐付けることです。

// 絵を入れる

そこで、``v-bind``で``BookTitle``コンポーネントに``currentBookId``という関数を紐付け、``currentBookId``は``addCount``・``removeCount``メソッドで計算した``bookId``の値を返却するようにしました。子コンポーネントでは``prop``で親コンポーネントからデータを取得・それを元にJSONデータの配列から値を取得します。
``addCount``の処理は、配列の長さと違っていたら値を1加算するようにすれば意図した動作になるだろう、と考えて実装しました。あとでわかりますが、これは間違った実装でした。``removeCount``の処理は、今持っている``bookId``（インデックスの値）が0以外であれば、1を減算するような実装を行いました。配列のインデックスの値は0から始まるので、こうすれば0以下のインデックスを指定することが無くなります。


#### App.vue（propしたい値を定義）

```JavaScript
  <div id="app">
    <div class="header">
      <title-header></title-header>
-       <book-title></book-title>
+       <book-title v-bind:id="currentBookId"></book-title>
    </div>
    <div class="main">
      <button class="back_button" type="button" v-on:click="removeCount">
         <img v-bind:src="left_arrow" alt="back">
       </button>
       <div class="left-contents">
         <book-image></book-image>
         <description-list></description-list>
       </div>
       <div class="right-contents">
         <book-description></book-description>
         <book-info></book-info>
         <booth-button></booth-button>
       </div>
       <button class="next_button" v-on:click="addCount">
         <img v-bind:src="right_arrow" alt="next">
       </button>
     </div>
     <page-footer></page-footer>
   </div>
 </template>

 <script>
 import 'normalize.css'
 import titleHeader from './components/Header.vue'
 import bookData from './assets/book-data.json'
 import bookDescription from './components/BookDescription.vue'
 import bookImage from './components/BookImage.vue'
 import bookInfo from './components/BookInfo.vue'
 import bookTitle from './components/BookTitle.vue'
 import boothButton from './components/BoothButton.vue'
 import descriptionList from './components/DescriptionList.vue'
 import pageFooter from './components/Footer.vue'
 export default {
  name: 'app',
-   props: ['currentBookId'],
  components: {
    'title-header': titleHeader,
    'book-description':bookDescription,
     'book-image': bookImage,
     'book-info': bookInfo,
     'book-title': bookTitle,
     'booth-button': boothButton,
     'description-list': descriptionList,
     'page-footer': pageFooter
   },
   data() {
     return {
       bookData: bookData,
       bookId: 0,
       left_arrow: require("./assets/arrow/baseline-keyboard_arrow_left-24px.svg"),
       right_arrow: require("./assets/arrow/baseline-keyboard_arrow_right-24px.svg")
    }
  },
+   computed: {
+      currentBookId: function() {
+      return this.bookId;
+     }
+   },
  methods: {
    addCount() {
      if (this.bookId !== bookData.length ) {
         return this.bookId++;
       } else {
         return;
       }
     },
     removeCount() {
       if (this.bookId !== 0) {
         return this.bookId--;
       } else {
        return 0;
      }
    }
   }
 }
</script>

### BookTitle.vue（propした値を取得してJSONを表示）

```JavaScript
<template>
  <div class="book-title">
    <div class="fanbook-title">
      <div class="fan-book" v-bind:title="bookTitle">{{ bookTitle }}</div>
      <div class="fan-book" v-bind:title="bookTitle" v-bind:id="currentBookId">{{ bookTitle }}</div>
    </div>
  </div>
</template>

 <script>
import bookData from '../assets/book-data.json'
export default {
  name: 'bookTitle',
  props: ['id'],
  data() {
    return {
      bookData: bookData
     }
   },
  computed: {
    bookTitle: function() {
      //JSONから本のタイトルを取得する
-       return bookData[0].book_title;
+       return bookData[this.id].book_title;
    }
  }
}
 </script>
//}


やっほー！できたー！と思ったのですが、@<tt>{BookTitle.vue}の@<tt>{<div class="fan-book" v-bind:title="bookTitle" v-bind:id="currentBookId">{{ bookTitle \}\}</div>}の部分に不具合があります。というのも、@<tt>{v-bind:id="currentBookId"}の部分の@<tt>{currentBookId}はundefinedになってしまうためです。
理由は、基本的に子コンポーネントが親コンポーネントの値を参照することはできないためです。@<tt>{currentBookId}という変数は@<tt>{App.vue}の中にしか存在しません。子コンポーネントの@<tt>{BookTitle.vue}からすると、「@<tt>{currentBookId}さんなんてしりませんわ」というように見えるのです。
そこで、該当部分を削除する変更を加えました。


//emlist{
XXX: BLOCK_HTML: YOU SHOULD REWRITE IT
<!-- 図を入れる -->
//}

=== BookTitle.vue（undefinedになってしまうので修正する）

//emlist[][JavaScript]{
<template>
  <div class="book-title">
    <div class="fanbook-title">
-       <div class="fan-book" v-bind:title="bookTitle" v-bind:id="currentBookId">{{ bookTitle }}</div>
+       <div class="fan-book" v-bind:title="bookTitle">{{ bookTitle }}</div>
    </div>
  </div>
</template>

これで、ボタンを押すとタイトルだけ切り替わるようになりました。

### 画像以外の部分はボタン操作で切り替えできるようにする

この要領で、他の子コンポーネントにも配列のインデックスの値を親コンポーネントで決めてpropする処理を記載しました。これはissue #25の最終的な実装です。

### App.vue（#25終了時）

```JavaScript

<template>
  <div id="app">
    <div class="header">
      <title-header></title-header>
      <book-title v-bind:id="currentBookId"></book-title>
    </div>
    <div class="main">
      <button class="back_button" type="button" v-on:click="removeCount">
        <img v-bind:src="left_arrow" alt="back">
      </button>
      <div class="left-contents">
        <book-image></book-image>
        <description-list v-bind:id="currentBookId"></description-list>
      </div>
      <div class="right-contents">
        <book-description v-bind:id="currentBookId"></book-description>
        <book-info v-bind:id="currentBookId"></book-info>
        <booth-button v-bind:id="currentBookId"></booth-button>
      </div>
      <button class="next_button" v-on:click="addCount">
        <img v-bind:src="right_arrow" alt="next">
      </button>
    </div>
    <page-footer></page-footer>
  </div>
</template>

<script>
import 'normalize.css'
import titleHeader from './components/Header.vue'
import bookData from './assets/book-data.json'
import bookDescription from './components/BookDescription.vue'
import bookImage from './components/BookImage.vue'
import bookInfo from './components/BookInfo.vue'
import bookTitle from './components/BookTitle.vue'
import boothButton from './components/BoothButton.vue'
import descriptionList from './components/DescriptionList.vue'
import pageFooter from './components/Footer.vue'

export default {
  name: 'app',
  components: {
    'title-header': titleHeader,
    'book-description':bookDescription,
    'book-image': bookImage,
    'book-info': bookInfo,
    'book-title': bookTitle,
    'booth-button': boothButton,
    'description-list': descriptionList,
    'page-footer': pageFooter
  },
  data() {
    return {
      bookData: bookData,
      bookId: 0,
      left_arrow: require("./assets/arrow/baseline-keyboard_arrow_left-24px.svg"),
      right_arrow: require("./assets/arrow/baseline-keyboard_arrow_right-24px.svg")
    }
  },
  computed: {
    currentBookId: function() {
      return this.bookId;
    }
  },
  methods: {
    addCount() {
      if (this.bookId !== bookData.length ) {
        return this.bookId++;
      } else {
        return;
      }
    },
    removeCount() {
      if (this.bookId !== 0) {
        return this.bookId--;
      } else {
        return 0;
      }
    }
  }
}
</script>

<style>
  #app {
    background-color: #E5E400;
  }
  .main {
    display: flex;
    margin: 20px;
  }

  .left-contents {
    width: 50%;
  }

  .right-contents {
    width: 50%;
  }
</style>

//}

=== BookDescription.vue（#25終了時）

//emlist[][JavaScript]{

<template>
  <div class="book-description">
    <div class="description" v-bind:description="descText">{{ descText }}</div>
  </div>
</template>

<script>
import bookData from '../assets/book-data.json'
export default {
  name: 'bookDescription',
  props: ['id'],
  data() {
    return {
      bookData: bookData
      }
  },
  computed: {
    descText : function() {
      return bookData[this.id].book_description
    }
  }
}
</script>

<style scoped>
  .book-description {
    margin-top: 20px;
    width: 570px;
  }
  .description {
    font-size: 16px;
    line-height: 26px;
  }
</style>


//}

=== BookInfo.vue（#25終了時）


コメントにもありますが、配列のインデックスを指定するときはpropで取得した値を利用し、配列内に入っている配列の値（発刊イベント・年月）は配列の1番目に入っていることが明示的なので@<tt>{arrayData[0]}でデータを取得するようにしています。


//emlist[][JavaScript]{

<template>
  <div class="book-info">
    <div class="basic-info">
      <div class="published-year">
        <h4>発刊年月・イベント名</h4>
        <ul>
          <li v-for="info in infoTexts" v-bind:key="info.id">
            {{ info }}
          </li>
        </ul>
      </div>
      <div class="posts-info">
        <h4>関連URL</h4>
        <ul>
          <li v-for="list in posts" v-bind:key="list.id"><a v-bind:href="list">{{ list }}</a></li>
        </ul>
      </div>
    </div>
  </div>
</template>

<script>
import bookData from '../assets/book-data.json'
export default {
  name: 'bookInfo',
  props: ['id'],
  data() {
    return {
      bookData: bookData
    }
  },
  computed: {
    infoTexts: function() {
      const info = [];
      // JSONからinfoTextsに必要なデータを抜く。データは1冊の本に1つしかないのがわかっているので、配列の一番目の値を取得する。
      const arrayData = bookData[this.id].book_info;
      const year = arrayData[0].year;
      const month = arrayData[0].month;
      const eventName = arrayData[0].event_name;
      // 定数から配列を作成する。
      info.push(year);
      info.push(month);
      info.push(eventName);
      return info;
    },
    posts: function() {
      // 最終的に返却したい配列を定義
      const url = [];
      // JSON内のbook＿urlの配列の長さ・配列のデータを取得し、本のURLの値だけを持っている配列を作成する。
      const arrayLength = bookData[this.id].book_url.length;
      const arrayData = bookData[this.id].book_url;
      for(let count = 0; count < arrayLength; count++){
        const data = arrayData[count].book_url_url;
        url.push(data);
      }
      return url;
    }
  }
}
</script>

<style scoped>
  .book-info {
    font-size: 16px;
    line-height: 26px;
  }
  .published-info {
    margin-right: 5px;
  }
  h4 {
    font-size: 20px;
  }
</style>


//}

=== BoothButton.vue（#25終了時）

//emlist[][JavaScript]{

<template>
  <div class="booth-button">
    <div class="button">
      <a v-bind:href="boothUrl"><p>電子版をポチる</p></a>
    </div>
  </div>
</template>

<script>
import bookData from '../assets/book-data.json'
export default {
  name: 'boothButton',
  props: ['id'],
  data() {
    return {
        bookData: bookData
    }
  },
  computed: {
    boothUrl: function() {
      // JSONからBOOTHのURLを取得して返却する
      return bookData[this.id].booth_url;
    }
  }
}
</script>

<style scoped>
  .booth-button {
    margin-top: 60px;
    height: 200px;
  }
  .button {
    display: flex;
    flex-direction: center;
    justify-content: center;
  }
  a {
    background-color: #32BDED;
    color: #E40067;
    font-size: 20px;
    font-weight: bold;
    height: 60px;
    text-align: center;
    text-decoration: none;
    width: 400px;

  }
</style>


//}

=== DescriptionList.vue（#25終了時）

//emlist[][JavaScript]{

<template>
  <div class="description-list">
    <div class="overview">
      <h3>KUSO POINT</h3>
      <ul>
        <li v-for="item in promotionData" v-bind:key="item.id">
          {{ item }}
        </li>
      </ul>
    </div>
  </div>
</template>

<script>
import bookData from '../assets/book-data.json'
export default {
  name: 'descriptionList',
  props: ['id'],
  data() {
    return {
      bookData: bookData
    }
  },
  computed: {
    promotionData: function() {
      // 繰り返し処理でoverviews_promotionの値を全部抜いて新しい配列を作り、returnする
      const arrayLength = bookData[this.id].overviews.length;
      const arrayData = bookData[this.id].overviews;
      const promotions = [];
      for (let count = 0; count < arrayLength; count++) {
        const data = arrayData[count].overviews_promotion;
        promotions.push(data);
      }
      return promotions;
    }
  }
}
</script>

<style scoped>
  .descriptiion-list {
    height: 250px;
    margin-top: 20px;
    width: 570px;
  }
  h3 {
    font-size: 24px;
  }
  li {
    font-size: 16px;
    line-height: 26px;
  }
</style>

//}


この状態で画面を表示し、ボタン操作をしてみます。すると、画面の情報が切り替わっているようです。



//image[25_finish][ボタンを押すと文字の情報が切り替わっている]{
//}




しかし、配列の最後のデータ（6番目のデータ）を表示した後に右側の矢印を押すと、エラーとなってボタン操作が一切効かなくなってしまいます。これは別のissueとして修正することにしました。



//image[25_bug][最後のデータを表示するとエラーが出ている]{
//}



== #33 同人誌の表紙・裏表紙画像を準備する


文字列の切り替えができるようになったため、同じ要領で画像の切り替えをできるようにします。画像をどうやって表示するかに悩みましたが、次の方針で進めることにしました。

 1. 配列のインデックス番号と同じ名前のディレクトリをassetsに作成する
 1. 表紙の画像は@<tt>{front.png}、背景の画像は@<tt>{back.png}に統一して各ディレクトリに保存する
 1. App.vueでインデックス番号をpropし、@<tt>{BookImage.vue}でpropされた情報を元に画像のパスを切り替える
 1. ボタン操作に応じて画像が切り替わる



prop地獄を避けたいということと、項目数がそこまで多くないことの2点から、この実装方針としました。@<tt>{prop}や@<tt>{$emit}は大変便利ですが、項目数が多くなってくると「これはどこの誰にpropしたんだっけ…」となってしまい管理が大変です。それに加えて今回はボタンを押したときに、同人誌の情報が一括で変化してもらわないと困ってしまいます。そこで、文字列の項目を表示しているものと同じデータを利用して画像のパスを決定するようにしました。早速実装してみます。



まず、App.vueへ@<tt>{prop}の処理を追加します。


//emlist[][JavaScript]{
      <div class="left-contents">
-         <book-image></book-image>
+         <book-image v-bind:id="currentBookId"></book-image>
        <description-list v-bind:id="currentBookId"></description-list>
      </div>
//}


次に、BookImage.vueへ@<tt>{prop}された値を取得する処理を記載します。


//emlist[][JavaScript]{
<script>
export default {
  name: "bookImage",
+   props: '[id]',
  data() {
    return {
      image_src: require("../assets/bookImage/1.png")
//}


このままでは@<tt>{ログと情報をレッツ・ラ・まぜまぜ！}のデータが割り当たったままです。そこでデータの割り当てをする処理も記載します。


//emlist[][JavaScript]{
<template>
  <div class="book-image">
-     <img :src="image_src" alt="bookImage">
+     <img :src="frontImage" alt="bookImage">
+     <img :src="backImage" alt="bookImage">
  </div>
</template>

<script>
export default {
  name: "bookImage",
-   props: '[id]',
-   data() {
-     return {
-       image_src: require("../assets/bookImage/1.png")
+   props: ['id'],
+   computed: {
+     frontImage: function() {
+       return require('../assets/bookImage/' + this.id + '/front.png')
+     },
+     backImage: function() {
+       return require('../assets/bookImage/' + this.id + '/back.png')
+     }
  }
}
//}


合わせて、画像をディレクトリに追加しました。@<tt>{assets}ディレクトリ構成は下記のようになりました。


//emlist[][bash]{
/
|____assets
| |____footer
| |____Twitter_Social_Icon_Rounded_Square_Color.svg
| |____GitHub-Mark.svg
| |____arrow
| |____baseline-keyboard_arrow_left-24px.svg
| |____baseline-keyboard_arrow_right-24px.svg
| |____bookImage
| |____0
| | |____back.png
| | |____front.png
| |____1
| | |____back.png
| | |____front.png
| |____4
| | |____back.png
| | |____front.png
| |____3
| | |____back.png
| | |____front.png
| |____2
| | |____back.png
| | |____front.png
| |____5
| | |____back.png
| | |____front.png
| |____book-data.json
//}


結果、ボタンを押すと画像も切り替えができるようになりました。ここまでくれば完成は目前です。



//image[33_finish][#33実装後]{
//}



== #34 idがJSONの最後だとundefinedになる


日本語がかなり乏しいissueのタイトルですが、表題の通りJSONの最後の値になると@<tt>{undefined}になってしまい何もできなくなってしまいます。これを修正しなければ公開することはできません。


=== エラー内容の確認を行う


まず初めに、エラー内容をよく見ることにします。


//emlist[][JavaScript]{
vue.runtime.esm.js?2b0e:587 [Vue warn]: Error in render: "TypeError: Cannot read property 'book_title' of undefined"

found in

---> <BookTitle> at src/components/BookTitle.vue
       <App> at src/App.vue
         <Root>
warn @ vue.runtime.esm.js?2b0e:587
logError @ vue.runtime.esm.js?2b0e:1733
globalHandleError @ vue.runtime.esm.js?2b0e:1728
handleError @ vue.runtime.esm.js?2b0e:1717
Vue._render @ vue.runtime.esm.js?2b0e:4542
updateComponent @ vue.runtime.esm.js?2b0e:2784
get @ vue.runtime.esm.js?2b0e:3138
run @ vue.runtime.esm.js?2b0e:3215
flushSchedulerQueue @ vue.runtime.esm.js?2b0e:2977
(anonymous) @ vue.runtime.esm.js?2b0e:1833
flushCallbacks @ vue.runtime.esm.js?2b0e:1754
vue.runtime.esm.js?2b0e:1737 TypeError: Cannot read property 'book_title' of undefined
    at VueComponent.bookTitle (BookTitle.vue?5d46:22)
    at Watcher.get (vue.runtime.esm.js?2b0e:3138)
    at Watcher.evaluate (vue.runtime.esm.js?2b0e:3245)
    at VueComponent.computedGetter [as bookTitle] (vue.runtime.esm.js?2b0e:3503)
    at Object.get (vue.runtime.esm.js?2b0e:1916)
    at Proxy.render (eval at ./node_modules/cache-loader/dist/cjs.js?{"cacheDirectory":"node_modules/.cache/vue-loader","cacheIdentifier":"dc1d07a8-vue-loader-template"}!./node_modules/vue-loader/lib/loaders/templateLoader.js?!./node_modules/cache-loader/dist/cjs.js?!./node_modules/vue-loader/lib/index.js?!./src/components/BookTitle.vue?vue&type=template&id=1a6b496f&scoped=true& (app.js:973), <anonymous>:10:64)
    at VueComponent.Vue._render (vue.runtime.esm.js?2b0e:4540)
    at VueComponent.updateComponent (vue.runtime.esm.js?2b0e:2784)
    at Watcher.get (vue.runtime.esm.js?2b0e:3138)
    at Watcher.run (vue.runtime.esm.js?2b0e:3215)
logError @ vue.runtime.esm.js?2b0e:1737
globalHandleError @ vue.runtime.esm.js?2b0e:1728
handleError @ vue.runtime.esm.js?2b0e:1717
Vue._render @ vue.runtime.esm.js?2b0e:4542
updateComponent @ vue.runtime.esm.js?2b0e:2784
get @ vue.runtime.esm.js?2b0e:3138
run @ vue.runtime.esm.js?2b0e:3215
flushSchedulerQueue @ vue.runtime.esm.js?2b0e:2977
(anonymous) @ vue.runtime.esm.js?2b0e:1833
flushCallbacks @ vue.runtime.esm.js?2b0e:1754
vue.runtime.esm.js?2b0e:587 [Vue warn]: Error in render: "Error: Cannot find module './6/front.png'"

found in

---> <BookImage> at src/components/BookImage.vue
       <App> at src/App.vue
         <Root>
warn @ vue.runtime.esm.js?2b0e:587
logError @ vue.runtime.esm.js?2b0e:1733
globalHandleError @ vue.runtime.esm.js?2b0e:1728
handleError @ vue.runtime.esm.js?2b0e:1717
Vue._render @ vue.runtime.esm.js?2b0e:4542
updateComponent @ vue.runtime.esm.js?2b0e:2784
get @ vue.runtime.esm.js?2b0e:3138
run @ vue.runtime.esm.js?2b0e:3215
flushSchedulerQueue @ vue.runtime.esm.js?2b0e:2977
(anonymous) @ vue.runtime.esm.js?2b0e:1833
flushCallbacks @ vue.runtime.esm.js?2b0e:1754
vue.runtime.esm.js?2b0e:1737 Error: Cannot find module './6/front.png'
    at webpackContextResolve (eval at ./src/assets/bookImage sync recursive ^\.\/.*\/front\.png$ (app.js:2410), <anonymous>:18:11)
    at webpackContext (eval at ./src/assets/bookImage sync recursive ^\.\/.*\/front\.png$ (app.js:2410), <anonymous>:12:11)
    at VueComponent.frontImage (BookImage.vue?4ee6:14)
    at Watcher.get (vue.runtime.esm.js?2b0e:3138)
    at Watcher.evaluate (vue.runtime.esm.js?2b0e:3245)
    at VueComponent.computedGetter [as frontImage] (vue.runtime.esm.js?2b0e:3503)
    at Object.get (vue.runtime.esm.js?2b0e:1916)
    at Proxy.render (eval at ./node_modules/cache-loader/dist/cjs.js?{"cacheDirectory":"node_modules/.cache/vue-loader","cacheIdentifier":"dc1d07a8-vue-loader-template"}!./node_modules/vue-loader/lib/loaders/templateLoader.js?!./node_modules/cache-loader/dist/cjs.js?!./node_modules/vue-loader/lib/index.js?!./src/components/BookImage.vue?vue&type=template&id=656096f2&scoped=true& (app.js:949), <anonymous>:9:35)
    at VueComponent.Vue._render (vue.runtime.esm.js?2b0e:4540)
    at VueComponent.updateComponent (vue.runtime.esm.js?2b0e:2784)
logError @ vue.runtime.esm.js?2b0e:1737
globalHandleError @ vue.runtime.esm.js?2b0e:1728
handleError @ vue.runtime.esm.js?2b0e:1717
Vue._render @ vue.runtime.esm.js?2b0e:4542
updateComponent @ vue.runtime.esm.js?2b0e:2784
get @ vue.runtime.esm.js?2b0e:3138
run @ vue.runtime.esm.js?2b0e:3215
flushSchedulerQueue @ vue.runtime.esm.js?2b0e:2977
(anonymous) @ vue.runtime.esm.js?2b0e:1833
flushCallbacks @ vue.runtime.esm.js?2b0e:1754
vue.runtime.esm.js?2b0e:587 [Vue warn]: Error in render: "TypeError: Cannot read property 'overviews' of undefined"

found in

---> <DescriptionList> at src/components/DescriptionList.vue
       <App> at src/App.vue
         <Root>
warn @ vue.runtime.esm.js?2b0e:587
logError @ vue.runtime.esm.js?2b0e:1733
globalHandleError @ vue.runtime.esm.js?2b0e:1728
handleError @ vue.runtime.esm.js?2b0e:1717
Vue._render @ vue.runtime.esm.js?2b0e:4542
updateComponent @ vue.runtime.esm.js?2b0e:2784
get @ vue.runtime.esm.js?2b0e:3138
run @ vue.runtime.esm.js?2b0e:3215
flushSchedulerQueue @ vue.runtime.esm.js?2b0e:2977
(anonymous) @ vue.runtime.esm.js?2b0e:1833
flushCallbacks @ vue.runtime.esm.js?2b0e:1754
vue.runtime.esm.js?2b0e:1737 TypeError: Cannot read property 'overviews' of undefined
    at VueComponent.promotionData (DescriptionList.vue?068c:27)
    at Watcher.get (vue.runtime.esm.js?2b0e:3138)
    at Watcher.evaluate (vue.runtime.esm.js?2b0e:3245)
    at VueComponent.computedGetter [as promotionData] (vue.runtime.esm.js?2b0e:3503)
    at Object.get (vue.runtime.esm.js?2b0e:1916)
    at Proxy.render (eval at ./node_modules/cache-loader/dist/cjs.js?{"cacheDirectory":"node_modules/.cache/vue-loader","cacheIdentifier":"dc1d07a8-vue-loader-template"}!./node_modules/vue-loader/lib/loaders/templateLoader.js?!./node_modules/cache-loader/dist/cjs.js?!./node_modules/vue-loader/lib/index.js?!./src/components/DescriptionList.vue?vue&type=template&id=51aab08c&scoped=true& (app.js:997), <anonymous>:13:20)
    at VueComponent.Vue._render (vue.runtime.esm.js?2b0e:4540)
    at VueComponent.updateComponent (vue.runtime.esm.js?2b0e:2784)
    at Watcher.get (vue.runtime.esm.js?2b0e:3138)
    at Watcher.run (vue.runtime.esm.js?2b0e:3215)
logError @ vue.runtime.esm.js?2b0e:1737
globalHandleError @ vue.runtime.esm.js?2b0e:1728
handleError @ vue.runtime.esm.js?2b0e:1717
Vue._render @ vue.runtime.esm.js?2b0e:4542
updateComponent @ vue.runtime.esm.js?2b0e:2784
get @ vue.runtime.esm.js?2b0e:3138
run @ vue.runtime.esm.js?2b0e:3215
flushSchedulerQueue @ vue.runtime.esm.js?2b0e:2977
(anonymous) @ vue.runtime.esm.js?2b0e:1833
flushCallbacks @ vue.runtime.esm.js?2b0e:1754
vue.runtime.esm.js?2b0e:587 [Vue warn]: Error in render: "TypeError: Cannot read property 'book_description' of undefined"

found in

---> <BookDescription> at src/components/BookDescription.vue
       <App> at src/App.vue
         <Root>
warn @ vue.runtime.esm.js?2b0e:587
logError @ vue.runtime.esm.js?2b0e:1733
globalHandleError @ vue.runtime.esm.js?2b0e:1728
handleError @ vue.runtime.esm.js?2b0e:1717
Vue._render @ vue.runtime.esm.js?2b0e:4542
updateComponent @ vue.runtime.esm.js?2b0e:2784
get @ vue.runtime.esm.js?2b0e:3138
run @ vue.runtime.esm.js?2b0e:3215
flushSchedulerQueue @ vue.runtime.esm.js?2b0e:2977
(anonymous) @ vue.runtime.esm.js?2b0e:1833
flushCallbacks @ vue.runtime.esm.js?2b0e:1754
vue.runtime.esm.js?2b0e:1737 TypeError: Cannot read property 'book_description' of undefined
    at VueComponent.descText (BookDescription.vue?a8d0:19)
    at Watcher.get (vue.runtime.esm.js?2b0e:3138)
    at Watcher.evaluate (vue.runtime.esm.js?2b0e:3245)
    at VueComponent.computedGetter [as descText] (vue.runtime.esm.js?2b0e:3503)
    at Object.get (vue.runtime.esm.js?2b0e:1916)
    at Proxy.render (eval at ./node_modules/cache-loader/dist/cjs.js?{"cacheDirectory":"node_modules/.cache/vue-loader","cacheIdentifier":"dc1d07a8-vue-loader-template"}!./node_modules/vue-loader/lib/loaders/templateLoader.js?!./node_modules/cache-loader/dist/cjs.js?!./node_modules/vue-loader/lib/index.js?!./src/components/BookDescription.vue?vue&type=template&id=2e2a9dd3&scoped=true& (app.js:937), <anonymous>:11:63)
    at VueComponent.Vue._render (vue.runtime.esm.js?2b0e:4540)
    at VueComponent.updateComponent (vue.runtime.esm.js?2b0e:2784)
    at Watcher.get (vue.runtime.esm.js?2b0e:3138)
    at Watcher.run (vue.runtime.esm.js?2b0e:3215)
logError @ vue.runtime.esm.js?2b0e:1737
globalHandleError @ vue.runtime.esm.js?2b0e:1728
handleError @ vue.runtime.esm.js?2b0e:1717
Vue._render @ vue.runtime.esm.js?2b0e:4542
updateComponent @ vue.runtime.esm.js?2b0e:2784
get @ vue.runtime.esm.js?2b0e:3138
run @ vue.runtime.esm.js?2b0e:3215
flushSchedulerQueue @ vue.runtime.esm.js?2b0e:2977
(anonymous) @ vue.runtime.esm.js?2b0e:1833
flushCallbacks @ vue.runtime.esm.js?2b0e:1754
vue.runtime.esm.js?2b0e:587 [Vue warn]: Error in render: "TypeError: Cannot read property 'book_info' of undefined"

found in

---> <BookInfo> at src/components/BookInfo.vue
       <App> at src/App.vue
         <Root>
warn @ vue.runtime.esm.js?2b0e:587
logError @ vue.runtime.esm.js?2b0e:1733
globalHandleError @ vue.runtime.esm.js?2b0e:1728
handleError @ vue.runtime.esm.js?2b0e:1717
Vue._render @ vue.runtime.esm.js?2b0e:4542
updateComponent @ vue.runtime.esm.js?2b0e:2784
get @ vue.runtime.esm.js?2b0e:3138
run @ vue.runtime.esm.js?2b0e:3215
flushSchedulerQueue @ vue.runtime.esm.js?2b0e:2977
(anonymous) @ vue.runtime.esm.js?2b0e:1833
flushCallbacks @ vue.runtime.esm.js?2b0e:1754
vue.runtime.esm.js?2b0e:1737 TypeError: Cannot read property 'book_info' of undefined
    at VueComponent.infoTexts (BookInfo.vue?4ab4:36)
    at Watcher.get (vue.runtime.esm.js?2b0e:3138)
    at Watcher.evaluate (vue.runtime.esm.js?2b0e:3245)
    at VueComponent.computedGetter [as infoTexts] (vue.runtime.esm.js?2b0e:3503)
    at Object.get (vue.runtime.esm.js?2b0e:1916)
    at Proxy.render (eval at ./node_modules/cache-loader/dist/cjs.js?{"cacheDirectory":"node_modules/.cache/vue-loader","cacheIdentifier":"dc1d07a8-vue-loader-template"}!./node_modules/vue-loader/lib/loaders/templateLoader.js?!./node_modules/cache-loader/dist/cjs.js?!./node_modules/vue-loader/lib/index.js?!./src/components/BookInfo.vue?vue&type=template&id=c839b832&scoped=true& (app.js:961), <anonymous>:14:22)
    at VueComponent.Vue._render (vue.runtime.esm.js?2b0e:4540)
    at VueComponent.updateComponent (vue.runtime.esm.js?2b0e:2784)
    at Watcher.get (vue.runtime.esm.js?2b0e:3138)
    at Watcher.run (vue.runtime.esm.js?2b0e:3215)
logError @ vue.runtime.esm.js?2b0e:1737
globalHandleError @ vue.runtime.esm.js?2b0e:1728
handleError @ vue.runtime.esm.js?2b0e:1717
Vue._render @ vue.runtime.esm.js?2b0e:4542
updateComponent @ vue.runtime.esm.js?2b0e:2784
get @ vue.runtime.esm.js?2b0e:3138
run @ vue.runtime.esm.js?2b0e:3215
flushSchedulerQueue @ vue.runtime.esm.js?2b0e:2977
(anonymous) @ vue.runtime.esm.js?2b0e:1833
flushCallbacks @ vue.runtime.esm.js?2b0e:1754
vue.runtime.esm.js?2b0e:587 [Vue warn]: Error in render: "TypeError: Cannot read property 'booth_url' of undefined"

found in

---> <BoothButton> at src/components/BoothButton.vue
       <App> at src/App.vue
         <Root>
warn @ vue.runtime.esm.js?2b0e:587
logError @ vue.runtime.esm.js?2b0e:1733
globalHandleError @ vue.runtime.esm.js?2b0e:1728
handleError @ vue.runtime.esm.js?2b0e:1717
Vue._render @ vue.runtime.esm.js?2b0e:4542
updateComponent @ vue.runtime.esm.js?2b0e:2784
get @ vue.runtime.esm.js?2b0e:3138
run @ vue.runtime.esm.js?2b0e:3215
flushSchedulerQueue @ vue.runtime.esm.js?2b0e:2977
(anonymous) @ vue.runtime.esm.js?2b0e:1833
flushCallbacks @ vue.runtime.esm.js?2b0e:1754
vue.runtime.esm.js?2b0e:1737 TypeError: Cannot read property 'booth_url' of undefined
    at VueComponent.boothUrl (BoothButton.vue?400f:22)
    at Watcher.get (vue.runtime.esm.js?2b0e:3138)
    at Watcher.evaluate (vue.runtime.esm.js?2b0e:3245)
    at VueComponent.computedGetter [as boothUrl] (vue.runtime.esm.js?2b0e:3503)
    at Object.get (vue.runtime.esm.js?2b0e:1916)
    at Proxy.render (eval at ./node_modules/cache-loader/dist/cjs.js?{"cacheDirectory":"node_modules/.cache/vue-loader","cacheIdentifier":"dc1d07a8-vue-loader-template"}!./node_modules/vue-loader/lib/loaders/templateLoader.js?!./node_modules/cache-loader/dist/cjs.js?!./node_modules/vue-loader/lib/index.js?!./src/components/BoothButton.vue?vue&type=template&id=37d0d088&scoped=true& (app.js:985), <anonymous>:10:36)
    at VueComponent.Vue._render (vue.runtime.esm.js?2b0e:4540)
    at VueComponent.updateComponent (vue.runtime.esm.js?2b0e:2784)
    at Watcher.get (vue.runtime.esm.js?2b0e:3138)
    at Watcher.run (vue.runtime.esm.js?2b0e:3215)
logError @ vue.runtime.esm.js?2b0e:1737
globalHandleError @ vue.runtime.esm.js?2b0e:1728
handleError @ vue.runtime.esm.js?2b0e:1717
Vue._render @ vue.runtime.esm.js?2b0e:4542
updateComponent @ vue.runtime.esm.js?2b0e:2784
get @ vue.runtime.esm.js?2b0e:3138
run @ vue.runtime.esm.js?2b0e:3215
flushSchedulerQueue @ vue.runtime.esm.js?2b0e:2977
(anonymous) @ vue.runtime.esm.js?2b0e:1833
flushCallbacks @ vue.runtime.esm.js?2b0e:1754
//}


原因は@<tt>{vue.runtime.esm.js?2b0e:587 [Vue warn]: Error in render: "TypeError: Cannot read property 'book_title' of undefined"}の部分が示しているようです。JSONの中の@<tt>{book_title}の値を特定できない、と推測することができます。
実際、同人誌は6種類しか存在しないので、7冊目を選択しようとしてもデータは存在しません。ですから@<strong>{0以上6以下の範囲であれば}インデックスの加算・減算処理を行うようにするべきです。



元の処理は、次のように記載されていました。


=== 今までのApp.vue

//emlist[][JavaScript]{
<script>
// import処理は省略

 export default {
   name: 'app',
   components: {
     'title-header': titleHeader,
     'book-description':bookDescription,
     'book-image': bookImage,
     'book-info': bookInfo,
     'book-title': bookTitle,
     'booth-button': boothButton,
     'description-list': descriptionList,
     'page-footer': pageFooter
   },
   data() {
     return {
       bookData: bookData,
       bookId: 0,
       left_arrow: require("./assets/arrow/baseline-keyboard_arrow_left-24px.svg"),
       right_arrow: require("./assets/arrow/baseline-keyboard_arrow_right-24px.svg")
     }
   },
   computed: {
     currentBookId: function() {
       return this.bookId;
     }
  },
  methods: {
    addCount() {
      if (this.bookId !== bookData.length ) {
        return this.bookId++;
      } else {
        return;
      }
    },
//}


この処理だと、@<tt>{bookId}が配列の長さ（＝ 6）以外のときはbookIdに1を加算する処理となります。つまり、@<strong>{bookIdが6}のときは加算処理がされず、それ以上の数は永遠に@<tt>{bookId}の中に代入されない＝@<strong>{6以上のインデックスは}指定されないようになります。
しかし、よく考えてください。@<strong>{配列のインデックスは0から始まります}。つまり、配列のデータ数が@<strong>{6}であれば、インデックスの最大値は@<strong>{5}です。今の処理は@<strong>{6}が@<tt>{bookId}に代入されます。しかし、実際は7番目のデータを取得する処理となります。これではデータを探せるはずがありません。



そこで、次のように処理を書き換えました。


//emlist[][JavaScript]{
  methods: {
    addCount() {
-       if (this.bookId !== bookData.length ) {
+       if (this.bookId < bookData.length - 1) {
        return this.bookId++;
-       } else {
-         return;
+       } else if (this.bookId >= bookData.length - 1) {
+           this.bookId = bookData.length - 1;
+           return this.bookId;
+         } else {
+           return;
      }
    },
//}


まず、bookIdが@<tt>{配列の長さ - 1}以下か判定します。@<tt>{配列の長さ - 1}以下であれば、@<tt>{bookId}に1を加算します。これにより、同人誌の次の情報を取得できますね。次に、@<tt>{else..if文}を使って@<tt>{配列の長さ - 1以下ではなかった場合}、@<tt>{bookId}が@<tt>{配列の長さ - 1}以上（＝ 5よりも大きい）になったかを調べます。@<tt>{if}分岐の処理で@<tt>{bookId}はどうしても@<tt>{6}になってしまうためです。その場合は加算処理を行わず、配列の長さと同じ値を@<tt>{bookId}へ代入し返却します。こうすることで、配列の長さの最大値をインデックスとして指定できるようになります。


=== #34で実装したApp.vue

//emlist[][JavaScript]{
<template>
  <div id="app">
    <div class="header">
      <title-header></title-header>
      <book-title v-bind:id="currentBookId"></book-title>
    </div>
    <div class="main">
      <button class="back_button" type="button" v-on:click="removeCount">
        <img v-bind:src="left_arrow" alt="back">
      </button>
      <div class="left-contents">
        <book-image v-bind:id="currentBookId"></book-image>
        <description-list v-bind:id="currentBookId"></description-list>
      </div>
      <div class="right-contents">
        <book-description v-bind:id="currentBookId"></book-description>
        <book-info v-bind:id="currentBookId"></book-info>
        <booth-button v-bind:id="currentBookId"></booth-button>
      </div>
      <button class="next_button" v-on:click="addCount">
        <img v-bind:src="right_arrow" alt="next">
      </button>
    </div>
    <page-footer></page-footer>
  </div>
</template>

<script>
import 'normalize.css'
import titleHeader from './components/Header.vue'
import bookData from './assets/book-data.json'
import bookDescription from './components/BookDescription.vue'
import bookImage from './components/BookImage.vue'
import bookInfo from './components/BookInfo.vue'
import bookTitle from './components/BookTitle.vue'
import boothButton from './components/BoothButton.vue'
import descriptionList from './components/DescriptionList.vue'
import pageFooter from './components/Footer.vue'

export default {
  name: 'app',
  components: {
    'title-header': titleHeader,
    'book-description':bookDescription,
    'book-image': bookImage,
    'book-info': bookInfo,
    'book-title': bookTitle,
    'booth-button': boothButton,
    'description-list': descriptionList,
    'page-footer': pageFooter
  },
  data() {
    return {
      bookData: bookData,
      bookId: 0,
      left_arrow: require("./assets/arrow/baseline-keyboard_arrow_left-24px.svg"),
      right_arrow: require("./assets/arrow/baseline-keyboard_arrow_right-24px.svg")
    }
  },
  computed: {
    currentBookId: function() {
      return this.bookId;
    }
  },
  methods: {
    addCount() {
      if (this.bookId < bookData.length -1) {
        return this.bookId++;
      } else if (this.bookId >= bookData.length - 1) {
          this.bookId = bookData.length - 1;
          return this.bookId;
        } else {
          return;
      }
    },
    removeCount() {
      if (this.bookId !== 0) {
        return this.bookId--;
      } else {
        return 0;
      }
    }
  }
}
</script>

<style>
  #app {
    background-color: #E5E400;
  }
  .main {
    display: flex;
    margin: 20px;
  }

  .left-contents {
    width: 50%;
  }

  .right-contents {
    width: 50%;
  }
</style>
//}


画面右下にエラーが出力されていないのが見えるでしょうか。これで最後の画像になってもボタンが効く状態を維持することができました。



//image[34_finish][不具合修正後の画面]{
//}



== 見た目や誤字を修正する


ボタンの見た目が個人的に気に食わなかったので@<tt>{＞}・@<tt>{＜}をemojiに置き換えました。KUSO感が高まったので大満足です。また、画面下の方の背景色が切れてしまう問題があったのでCSSを調整しました。また、@<tt>{favicon}も自作して@<tt>{assets}ディレクトリに配置しました。
また、『魔法つかいプリキュア！』表記が『魔法使いプリキュア！』になっていたため修正しました。 [^maho-girls]



[^maho-girls]: 事前にサイトを身内の人に見せたんですが「魔法つかいは@<tt>{つかい}のところひらがなだから」と怒られました。漢字変換されてたのに気づけなかった…。大変申し訳ありませんでした。


== 最終的なコード


これが完成系のサイトです。はじめにも出てきましたね。



//image[finish][最終的なWebサイト]{
//}




最終的なコードを全て記載します。


=== App.vue

//emlist[][JavaScript]{
<template>
  <div id="app">
    <div class="header">
      <title-header></title-header>
      <book-title v-bind:id="currentBookId"></book-title>
    </div>
    <div class="main">
      <button class="back-button" type="button" v-on:click="removeCount">
        👈
      </button>
      <div class="left-contents">
        <book-image v-bind:id="currentBookId"></book-image>
        <description-list v-bind:id="currentBookId"></description-list>
      </div>
      <div class="right-contents">
        <book-description v-bind:id="currentBookId"></book-description>
        <book-info v-bind:id="currentBookId"></book-info>
        <booth-button v-bind:id="currentBookId"></booth-button>
      </div>
      <button class="next-button" v-on:click="addCount">
        👉
      </button>
    </div>
    <page-footer></page-footer>
  </div>
</template>

<script>
import 'normalize.css'
import titleHeader from './components/Header.vue'
import bookData from './assets/book-data.json'
import bookDescription from './components/BookDescription.vue'
import bookImage from './components/BookImage.vue'
import bookInfo from './components/BookInfo.vue'
import bookTitle from './components/BookTitle.vue'
import boothButton from './components/BoothButton.vue'
import descriptionList from './components/DescriptionList.vue'
import pageFooter from './components/Footer.vue'

export default {
  name: 'app',
  components: {
    'title-header': titleHeader,
    'book-description':bookDescription,
    'book-image': bookImage,
    'book-info': bookInfo,
    'book-title': bookTitle,
    'booth-button': boothButton,
    'description-list': descriptionList,
    'page-footer': pageFooter
  },
  data() {
    return {
      bookData: bookData,
      bookId: 0,
      left_arrow: require("./assets/arrow/baseline-keyboard_arrow_left-24px.svg"),
      right_arrow: require("./assets/arrow/baseline-keyboard_arrow_right-24px.svg")
    }
  },
  computed: {
    currentBookId: function() {
      return this.bookId;
    }
  },
  methods: {
    addCount() {
      if (this.bookId < bookData.length -1) {
        return this.bookId++;
      } else if (this.bookId >= bookData.length - 1) {
          this.bookId = bookData.length - 1;
          return this.bookId;
        } else {
          return;
      }
    },
    removeCount() {
      if (this.bookId !== 0) {
        return this.bookId--;
      } else {
        return 0;
      }
    }
  }
}
</script>

<style>
  #app {
    background-color: #e5e400;
    height: 100%;
  }
  .main {
    display: flex;
    margin: 20px;
  }

  button {
    background-color: #3ebde0;
    border: none;
    height: 30px;
    width: 30px;
  }

  .book-title {
    margin: 10px;
  }

  .back-button {
    margin-top: 40%;
    margin-right: 10px;
  }

  .next-button {
    margin-top: 40%;
    margin-left: 10px;
  }

  .left-contents {
    width: 50%;
  }

  .book-image {
    margin-left: 10px;
  }

  .description-list {
    margin-left: 10px;
  }

  .right-contents {
    width: 50%;
  }
</style>
//}

=== BookDescription.vue

//emlist[][JavaScript]{
<template>
  <div class="book-description">
    <div class="description" v-bind:description="descText">{{ descText }}</div>
  </div>
</template>

<script>
import bookData from '../assets/book-data.json'
export default {
  name: 'bookDescription',
  props: ['id'],
  data() {
    return {
      bookData: bookData
      }
  },
  computed: {
    descText : function() {
      return bookData[this.id].book_description
    }
  }
}
</script>

<style scoped>
  .book-description {
    color: #3a3839;
    margin-top: 20px;
    width: 570px;
  }
  .description {
    font-size: 16px;
    line-height: 26px;
  }
</style>
//}

=== BookImage.vue

//emlist[][JavaScript]{
<template>
  <div class="book-image">
    <img :src="frontImage" alt="bookImage">
    <img :src="backImage" alt="bookImage">
  </div>
</template>

<script>
export default {
  name: "bookImage",
  props: ['id'],
  computed: {
    frontImage: function() {
      return require('../assets/bookImage/' + this.id + '/front.png')
    },
    backImage: function() {
      return require('../assets/bookImage/' + this.id + '/back.png')
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
//}

=== BookInfo.vue

//emlist[][JavaScript]{
<template>
  <div class="book-info">
    <div class="basic-info">
      <div class="published-year">
        <h4>発刊年月・イベント名</h4>
        <ul>
          <li v-for="info in infoTexts" v-bind:key="info.id">
            {{ info }}
          </li>
        </ul>
      </div>
      <div class="posts-info">
        <h4>関連URL</h4>
        <ul>
          <li v-for="list in posts" v-bind:key="list.id"><a v-bind:href="list">{{ list }}</a></li>
        </ul>
      </div>
    </div>
  </div>
</template>

<script>
import bookData from '../assets/book-data.json'
export default {
  name: 'bookInfo',
  props: ['id'],
  data() {
    return {
      bookData: bookData
    }
  },
  computed: {
    infoTexts: function() {
      const info = [];
      // JSONからinfoTextsに必要なデータを抜く。データは1冊の本に1つしかないのがわかっているので、配列の一番目の値を取得する。
      const arrayData = bookData[this.id].book_info;
      const year = arrayData[0].year;
      const month = arrayData[0].month;
      const eventName = arrayData[0].event_name;
      // 定数から配列を作成する。
      info.push(year);
      info.push(month);
      info.push(eventName);
      return info;
    },
    posts: function() {
      // 最終的に返却したい配列を定義
      const url = [];
      // JSON内のbook＿urlの配列の長さ・配列のデータを取得し、本のURLの値だけを持っている配列を作成する。
      const arrayLength = bookData[this.id].book_url.length;
      const arrayData = bookData[this.id].book_url;
      for(let count = 0; count < arrayLength; count++){
        const data = arrayData[count].book_url_url;
        url.push(data);
      }
      return url;
    }
  }
}
</script>

<style scoped>
  .book-info {
    color: #3a3839;
    font-size: 16px;
    line-height: 26px;
  }
  .published-info {
    margin-right: 5px;
  }
  h4 {
    font-size: 20px;
  }
</style>
//}

=== BookTitle.vue

//emlist[][JavaScript]{
<template>
  <div class="book-title">
    <div class="fanbook-title">
      <div class="fan-book" v-bind:title="bookTitle">{{ bookTitle }}</div>
    </div>
  </div>
</template>

<script>
import bookData from '../assets/book-data.json'
export default {
  name: 'bookTitle',
  props: ['id'],
  data() {
    return {
      bookData: bookData
    }
  },
  computed: {
    bookTitle: function() {
      //JSONから本のタイトルを取得する
      return bookData[this.id].book_title;
    }
  }
}
</script>

<style scoped>
  .book-title {
    background-color: #e5e400;
    color: #3ebde0;
    font-size: 30px;
  }
</style>
//}

=== BoothButton.vue

//emlist[][JavaScript]{
<template>
  <div class="booth-button">
    <div class="button">
      <a v-bind:href="boothUrl"><p>電子版をポチる</p></a>
    </div>
  </div>
</template>

<script>
import bookData from '../assets/book-data.json'
export default {
  name: 'boothButton',
  props: ['id'],
  data() {
    return {
        bookData: bookData
    }
  },
  computed: {
    boothUrl: function() {
      // JSONからBOOTHのURLを取得して返却する
      return bookData[this.id].booth_url;
    }
  }
}
</script>

<style scoped>
  .booth-button {
    margin-top: 60px;
    height: 200px;
  }
  .button {
    display: flex;
    flex-direction: center;
    justify-content: center;
  }
  a {
    background-color: #3ebde0;
    color: #e40067;
    font-size: 20px;
    font-weight: bold;
    height: 60px;
    text-align: center;
    text-decoration: none;
    width: 400px;

  }
</style>
//}

=== DescriptionList.vue

//emlist[][JavaScript]{
<template>
  <div class="description-list">
    <div class="overview">
      <h3>KUSO POINT</h3>
      <ul>
        <li v-for="item in promotionData" v-bind:key="item.id">
          {{ item }}
        </li>
      </ul>
    </div>
  </div>
</template>

<script>
import bookData from '../assets/book-data.json'
export default {
  name: 'descriptionList',
  props: ['id'],
  data() {
    return {
      bookData: bookData
    }
  },
  computed: {
    promotionData: function() {
      // 繰り返し処理でoverviews_promotionの値を全部抜いて新しい配列を作り、returnする
      const arrayLength = bookData[this.id].overviews.length;
      const arrayData = bookData[this.id].overviews;
      const promotions = [];
      for (let count = 0; count < arrayLength; count++) {
        const data = arrayData[count].overviews_promotion;
        promotions.push(data);
      }
      return promotions;
    }
  }
}
</script>

<style scoped>
  .descriptiion-list {
    color: #3a3839;
    height: 250px;
    margin-top: 20px;
    width: 570px;
  }

  h3 {
    font-size: 24px;
  }

  li {
    font-size: 16px;
    line-height: 26px;
  }
</style>
//}

=== Footer.vue

//emlist[][JavaScript]{
<template>
  <div class="page-footer">
    <div class="icons">
      <a href="https://twitter.com/froakie0021" target="blank"><img :src="twitter_src" alt="twitter"></a>
      <a href="https://github.com/MofuMofu2/portfolio-vue" target="blank"><img :src="github_src" alt="github"></a>
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
      twitter_src: require("../assets/footer/Twitter_Social_Icon_Rounded_Square_Color.svg"),
      github_src: require("../assets/footer/GitHub-Mark.svg")
    }
  }
}
</script>

<style scoped>
  .page-footer {
    background-color: #e5e400;
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
    color: #3ebde0;
    text-align: center;
  }

</style>
//}

=== Header.vue

//emlist[][JavaScript]{
<template>
  <div class="tite-header">
    <h1>{{ title }}</h1>
  </div>
</template>

<script>
export default {
  name: 'titleHeader',
  data() {
    return {
      title: '同人誌が…溢れ出す！'
    }
  }
}
</script>

<style scoped>
  h1 {
    align-items: center;
    background-color: #3ebde0;
    color: #e5e400;
    display: flex;
    font-size: 28px;
    height: 50px;
    margin: 0;
  }
</style>
//}

=== main.js

//emlist[][JavaScript]{
import Vue from 'vue'
import App from './App.vue'

Vue.config.productionTip = false

new Vue({
  render: h => h(App),
}).$mount('#app')
//}


最後に、これらのアプリケーションをデプロイしましょう。


== 参考URL

=== Issues
 * https://github.com/MofuMofu2/portfolio-vue/issues/33
 * https://github.com/MofuMofu2/portfolio-vue/issues/34


=== Pull Requests
 * https://github.com/MofuMofu2/portfolio-vue/pull/32
 * https://github.com/MofuMofu2/portfolio-vue/pull/35
 * https://github.com/MofuMofu2/portfolio-vue/pull/37
 * https://github.com/MofuMofu2/portfolio-vue/pull/38


=== undefinedの〇〇

==== Cannot read property 'fuga' of undefined　とは

//quote{
Cannot read property 'fuga' of undefined
というエラーが出たりする。
これが出た時に、
「fugaがundefinedなんですね！」
というと
「fuck」
という顔をされてしまい、説明をいただきました。

//}


まっっっっっっっっっっっっっっっっっっったく同じことを思いました。が、このエラーが出ているときは〇〇の処理の1つ前の処理で@<tt>{undefined}になってしまっていることがほとんどです。1つ前の処理が正しくできているか？を標準出力/デバッガーを使って確認しましょう。

 * https://qiita.com/nakamaru/items/3e11bb5b0d586a6d5545

