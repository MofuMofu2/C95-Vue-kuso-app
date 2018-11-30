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

