<template>
  <div
    class="rounded-borders"
    style="background-color: rgba(0, 0, 0, 0.5); padding: 10px"
  >
    <q-splitter v-model="splitterModel" :limits="[15, 85]">
      <template v-slot:before>
        <q-tabs v-model="tabModel" vertical class="text-white">
          <q-tab name="info" icon="fas fa-heartbeat" :label="$t('概览')" />
          <q-tab name="news" icon="fas fa-newspaper" :label="$t('新闻')" />
          <q-tab name="forum" icon="fas fa-comments" :label="$t('社区')" />
          <q-tab name="shop" icon="fas fa-gifts" :label="$t('商城')" />
          <q-tab name="download" icon="fas fa-download" :label="$t('下载')" />
          <q-tab name="about" icon="fas fa-id-card" :label="$t('关于')" />
          <q-tab name="account" icon="fas fa-info-circle" :label="$t('账号')" />
        </q-tabs>
      </template>

      <template v-slot:after>
        <q-tab-panels
          class="text-white transparent"
          v-model="tabModel"
          animated
          transition-prev="jump-up"
          transition-next="jump-up"
        >
          <q-tab-panel name="info">
            <q-scroll-area style="height: 500px">
              <div class="text-h6">{{ $t("服务器时间") }}：{{ os.time }}</div>
              <br />
              <div class="text-h6">{{ $t("运行时长") }}：{{ os.runtime }}</div>
              <br />
              <q-input
                filled
                dark
                v-model="config.server"
                :label="$t('游戏服务器地址')"
              />
              <br />
              <q-input
                filled
                dark
                v-model="config.launcher"
                :label="$t('启动器目录')"
              />
              <br />
              <q-input
                filled
                dark
                v-model="config.version"
                :label="$t('版本')"
              />
              <br />
              <q-input
                filled
                dark
                v-model="config.update"
                :label="$t('补丁目录')"
              />
              <br />
              <q-input filled dark v-model="config.forum" :label="$t('社区')" />
              <q-input filled dark v-model="config.discord" label="Discord" />
              <q-input filled dark v-model="config.facebook" label="Facebook" />
              <q-input filled dark v-model="config.twitter" label="Twitter" />
              <q-input filled dark v-model="config.weibo" :label="$t('微博')" />
              <q-input
                filled
                dark
                v-model="config.weixin"
                :label="$t('微信')"
              />
              <br />
              <q-btn
                class="fill"
                color="primary"
                :label="$t('保存')"
                @click="onSave"
              />
            </q-scroll-area>
          </q-tab-panel>

          <q-tab-panel name="news">
            <q-scroll-area style="height: 500px">
              <adminNews />
            </q-scroll-area>
          </q-tab-panel>

          <q-tab-panel name="forum"></q-tab-panel>

          <q-tab-panel name="shop"></q-tab-panel>

          <q-tab-panel name="download">
            <adminMarkdown :database="markdown.download" type="download" />
          </q-tab-panel>

          <q-tab-panel name="about">
            <adminMarkdown :database="markdown.about" type="about" />
          </q-tab-panel>

          <q-tab-panel name="account"></q-tab-panel>
        </q-tab-panels>
      </template>
    </q-splitter>
  </div>
</template>

<script>
import moment from "moment";

export default {
  name: "PageAdmin",

  components: {
    adminNews: () => import("pages/AdminNews.vue"),
    adminMarkdown: () => import("pages/AdminMarkdown.vue")
  },

  data() {
    return {
      splitterModel: 10,
      tabModel: "info",
      os: {
        time: null,
        runtime: null
      },
      config: {
        version: null,
        update: null,
        server: null,
        launcher: null,
        forum: null,
        discord: null,
        facebook: null,
        twitter: null,
        weibo: null,
        weixin: null
      },
      markdown: {
        download: null,
        about: null
      },
      timer: null
    };
  },

  methods: {
    onSave() {
      this.$axios.post(this.$store.state.custom.ip + "/overview", {
        token: this.$q.sessionStorage.getItem("token"),
        methond: "save",
        launcher: this.config.launcher,
        version: this.config.version,
        update: this.config.update,
        server: this.config.server,
        forum: this.config.forum,
        discord: this.config.discord,
        facebook: this.config.facebook,
        twitter: this.config.twitter,
        weibo: this.config.weibo,
        weixin: this.config.weixin
      });
    }
  },

  created() {
    if (!this.$store.state.custom.logined) this.$router.push("/");

    let timeUnix, runtimeUnix;

    this.$axios
      .post(this.$store.state.custom.ip + "/overview", {
        token: this.$q.sessionStorage.getItem("token")
      })
      .then(response => {
        moment.locale("zh-cn");

        timeUnix = response.data.msg.time;
        runtimeUnix = response.data.msg.runtime;
        this.os.time = moment.unix(timeUnix).format("YYYY年 MMMM Do, h:mm:ss");
        this.os.runtime = moment
          .duration(runtimeUnix - timeUnix, "s")
          .humanize();

        this.timer = window.setInterval(() => {
          timeUnix = moment(timeUnix).add(1, "ms");
          this.os.time = moment
            .unix(timeUnix)
            .format("YYYY年 MMMM Do, h:mm:ss");
          this.os.runtime = moment
            .duration(runtimeUnix - timeUnix, "s")
            .humanize();
        }, 1000);

        this.config.version = response.data.msg.version;
        this.config.update = response.data.msg.update;
        this.config.launcher = response.data.msg.launcher;
        this.config.server = response.data.msg.server;
        this.config.forum = response.data.msg.forum;
        this.config.discord = response.data.msg.discord;
        this.config.facebook = response.data.msg.facebook;
        this.config.twitter = response.data.msg.twitter;
        this.config.weibo = response.data.msg.weibo;
        this.config.weixin = response.data.msg.weixin;
      });

    this.$axios
      .get(this.$store.state.custom.ip + "/markdown")
      .then(response => {
        this.markdown.download = response.data.msg[0].download;
      });

    this.$axios
      .get(this.$store.state.custom.ip + "/markdown")
      .then(response => {
        this.markdown.about = response.data.msg[0].about;
      });
  },

  destroyed() {
    window.clearTimeout(this.timer);
  }
};
</script>

<style></style>
