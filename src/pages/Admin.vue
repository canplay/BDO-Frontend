<template>
  <div class="rounded-borders" style="background-color: rgba(0, 0, 0, 0.5); padding: 10px">
    <q-splitter v-model="splitterModel" :limits="[15, 85]">
      <template v-slot:before>
        <q-tabs v-model="tabModel" vertical class="text-white">
          <q-tab name="info" icon="mail" label="概览" />
          <q-tab name="news" icon="alarm" label="新闻页面" />
          <q-tab name="forum" icon="alarm" label="社区页面" />
          <q-tab name="shop" icon="alarm" label="商城页面" />
          <q-tab name="download" icon="alarm" label="下载页面" />
          <q-tab name="about" icon="alarm" label="关于页面" />
          <q-tab name="account" icon="movie" label="账号管理" />
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
            <q-card class="bg-black">
              <q-card-section>
                <div class="text-h6">服务器时间：{{ os.time }}</div>
                <br />
                <div class="text-h6">运行时长：{{ os.runtime }}</div>
                <br />
                <q-input filled dark v-model="launcher" label="启动器目录" />
                <br />
                <q-input filled dark v-model="update.version" label="版本" />
                <br />
                <q-input filled dark v-model="update.path" label="补丁目录" />
                <br />
                <q-btn class="fill" color="primary" label="保存" />
              </q-card-section>
            </q-card>
          </q-tab-panel>

          <q-tab-panel name="news"></q-tab-panel>

          <q-tab-panel name="forum"></q-tab-panel>

          <q-tab-panel name="shop"></q-tab-panel>

          <q-tab-panel name="download"></q-tab-panel>

          <q-tab-panel name="about"></q-tab-panel>

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

  data() {
    return {
      splitterModel: 10,
      tabModel: "info",
      os: {
        time: null,
        runtime: null
      },
      launcher: null,
      update: {
        version: null,
        path: null
      },
      timer: null
    };
  },

  created() {
    if (!this.$store.state.custom.logined) this.$router.push("/");

    let timeUnix, runtimeUnix;

    this.$axios
      .post(this.$store.state.custom.ip + "/info", {
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

        this.update.version = response.data.msg.version;
        this.update.path = response.data.msg.update;
        this.launcher = response.data.msg.launcher;
      });
  },

  destroyed() {
    window.clearTimeout(this.timer);
  }
};
</script>

<style></style>
