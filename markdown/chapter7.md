# デプロイと反省

配信先は[Netlify](https://www.netlify.com/)としました。GitHubと連携できる点・すぐにデプロイができる点・無料でHTTPS対応をやってくれる点・VueでもNetlifyを利用したデプロイが考慮されているからです。

## アカウントの作成

Netlifyは基本的にホスティングされたGitリポジトリをビルド・デプロイします。アカウントを作るときにGitリポジトリをホスティングするサービスが基本となっている点からも特徴を伺うことができますね。今回はGitHubの``MofuMofu2``というアカウントをNetlifyに連携したいと思います。

![連携するアカウントを選択する](C95-vue-and-nuxt/images/chapter7/signup.png)

次に、どのGitリポジトリを連携するか選択します。今回は``portfolio-vue``リポジトリだけをデプロイする予定なので、``Only select repositories``を選択します。

![特定のGitリポジトリを連携する](C95-vue-and-nuxt/images/chapter7/select_repo.png)

すると、次のような画面が表示されます。

![ログイン後](C95-vue-and-nuxt/images/chapter7/select_repo.png)

## デプロイする

はじめに、どのGitリポジトリをデプロイするか決定します。今回はGitHubのリポジトリをデプロイします。

![どのリポジトリをデプロイするか決める](C95-vue-and-nuxt/images/chapter7/create_new_site_01.png)

次に、デプロイするために必要な情報を記載します。

- デプロイする対象のブランチ[^deploy-branch]
- ビルドコマンド
- 公開用ディレクトリ名（``dist``）

今回はnpmでパッケージ管理をしているので、ビルドコマンドは``npm run build``となります。デプロイコマンドは自身の環境によって異なるため、Vue CLIの[公式ドキュメント](https://cli.vuejs.org/guide/deployment.html)（``https://cli.vuejs.org/guide/deployment.html``）を確認してください。AWSやGitHub Pagesへのデプロイ方法もしっかりと記載があります。

![デプロイコマンドなどの指定](C95-vue-and-nuxt/images/chapter7/create_new_site_02.png)

[^deploy-branch]: git-flowに沿って開発をするのであれば、masterブランチをデプロイするのが一般的だと思います。

これだけでデプロイすることができます。デプロイ後はURLの変更・独自ドメインの設定などをすることが可能です。

これでやっとアプリケーションは完成！となりました。

## 一連の作業を振り返る

## 参考URL

### Deployment

- https://cli.vuejs.org/guide/deployment.html