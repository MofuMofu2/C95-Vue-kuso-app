# 完成形に近づける

やっとJSONからデータを取得することができました。しかし、これでは他の同人誌は一生いなかったことにされてしまいます。それは許せません。カルーセルの処理を入れる/前後に進むボタンを追加するなどしてデータを切り替えできるようにします。

## #25 情報を切り替える実装を追加

まずはじめに、前後に切り替えるためのボタンを実装します。Googleが提供している[Material Design-Icons](https://material.io/tools/icons/?style=baseline)（``https://material.io/tools/icons/?style=baseline``）から、切り替えボタン用の画像アイコンを取得しました。ライセンスはApache Licenceなので、Material Designのアイコンを利用したという情報をアプリケーションのどこかに入れればライセンス違反にはなりません。
最終的にはemojiに置き換えたため、今回は記述しませんでした。

```JavaScript
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
```

![ボタンを追加した](/Users/mofumofu/git/C95-vue-and-nuxt/images/chapter6/add_button.png)

見た目は見なかったことにします。

### どの同人誌の情報を表示するか判断できるようにする

今、JSONから本のタイトル・参考URLの情報を取得するときは次のような実装となっています。

```JavaScript
// JSONの配列の1番目のインデックスからbook_titleの情報を取得する
bookData[0].book_title
```

JSONのデータ配列の1番目のインデックスからbook_titleの情報を取得する、というのが具体的な実装です。ということは、JSONのデータ配列の2番目、3番目…というように配列のインデックス名を変更できれば、本の情報を切り替えることができるはずです。どうやって切り替えればいいのでしょう？

今回はボタンを押したときに画面内全てのデータを切り替える必要があります。ということは、配列のインデックスの値を一度に切り替えなくてはいけません。そこで``prop``の利用を検討しました。これは第5章の``JSONからデータを取得する方法を探す``で述べたとおりです。

まずはじめに、``App.vue``で配列のインデックスの値を定義する必要があります。その実装を行いました。ボタンがおされたときに値が変わるようにしたいため、``v-on``を利用してクリックされたときにイベントを検知し、``addCount``または``removeCount``メソッドを呼び出してインデックスの値を変化させる処理を呼び出します。

```JavaScript
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
```

やっほー！できたー！と思ったのですが、``BookTitle.vue``の``<div class="fan-book" v-bind:title="bookTitle" v-bind:id="currentBookId">{{ bookTitle }}</div>``の部分に不具合があります。というのも、``v-bind:id="currentBookId"``の部分の``currentBookId``はundefinedになってしまうためです。
理由は、基本的に子コンポーネントが親コンポーネントの値を参照することはできないためです。``currentBookId``という変数は``App.vue``の中にしか存在しません。子コンポーネントの``BookTitle.vue``からすると、「``currentBookId``さんなんてしりませんわ」というように見えるのです。
そこで、該当部分を削除する変更を加えました。

<!-- 図を入れる -->

### BookTitle.vue（undefinedになってしまうので修正する）

```JavaScript
<template>
  <div class="book-title">
    <div class="fanbook-title">
-       <div class="fan-book" v-bind:title="bookTitle" v-bind:id="currentBookId">{{ bookTitle }}</div>
+       <div class="fan-book" v-bind:title="bookTitle">{{ bookTitle }}</div>
    </div>
  </div>
</template>

これで、ボタンを押すとタイトルだけ切り替わるようになりました。

// キャプチャ取れるならとる

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

```

### BookDescription.vue（#25終了時）

```JavaScript

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


```

### BookInfo.vue（#25終了時）

コメントにもありますが、配列のインデックスを指定するときはpropで取得した値を利用し、配列内に入っている配列の値（発刊イベント・年月）は配列の1番目に入っていることが明示的なので``arrayData[0]``でデータを取得するようにしています。

```JavaScript

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


```

### BoothButton.vue（#25終了時）

```JavaScript

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


```

### DescriptionList.vue（#25終了時）

```JavaScript

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

```

この状態で画面を表示し、ボタン操作をしてみます。すると、画面の情報が切り替わっているようです。

![ボタンを押すと文字の情報が切り替わっている](C95-vue-and-nuxt/images/chapter6/#25_finish.png)

しかし、配列の最後のデータ（6番目のデータ）を表示した後に右側の矢印を押すと、エラーとなってボタン操作が一切効かなくなってしまいます。これは別のissueとして修正することにしました。

![最後のデータを表示するとエラーが出ている](C95-vue-and-nuxt/images/chapter6/#25_bug.png)

https://github.com/MofuMofu2/portfolio-vue/pull/32

## #33 同人誌の表紙・裏表紙画像を準備する

https://github.com/MofuMofu2/portfolio-vue/issues/33

文字列の切り替えができるようになったため、同じ要領で画像の切り替えをできるようにします。画像をどうやって表示するかに悩みましたが、次の方針で進めることにしました。

1. 配列のインデックス番号と同じ名前のディレクトリをassetsに作成する
2. 表紙の画像は``front.png``、背景の画像は``back.png``に統一して各ディレクトリに保存する
3. App.vueでインデックス番号をpropし、``BookImage.vue``でpropされた情報を元に画像のパスを切り替える
4. ボタン操作に応じて画像が切り替わる

prop地獄を避けたいということと、項目数がそこまで多くないことの2点から、この実装方針としました。``prop``や``$emit``は大変便利ですが、項目数が多くなってくると「これはどこの誰にpropしたんだっけ…」となってしまい管理が大変です。それに加えて今回はボタンを押したときに、同人誌の情報が一括で変化してもらわないと困ってしまいます。そこで、文字列の項目を表示しているものと同じデータを利用して画像のパスを決定するようにしました。早速実装してみます。

まず、App.vueへ``prop``の処理を追加します。

```JavaScript
      <div class="left-contents">
-         <book-image></book-image>
+         <book-image v-bind:id="currentBookId"></book-image>
        <description-list v-bind:id="currentBookId"></description-list>
      </div>
```

次に、BookImage.vueへ``prop``された値を取得する処理を記載します。

```JavaScript
<script>
export default {
  name: "bookImage",
+   props: '[id]',
  data() {
    return {
      image_src: require("../assets/bookImage/1.png")
```

このままでは``ログと情報をレッツ・ラ・まぜまぜ！``のデータが割り当たったままです。そこでデータの割り当てをする処理も記載します。

```JavaScript
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
```

合わせて、画像をディレクトリに追加しました。``assets``ディレクトリ構成は下記のようになりました。

```bash
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
```

