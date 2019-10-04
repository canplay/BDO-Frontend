<template>
  <q-layout>
    <q-img src="statics/bg.png" class="fixed-center fit" style="z-index: -1" />

    <div style="margin-left: 5%; margin-right: 5%">
      <div class="column">
        <div class="col" style="height: 10px" />

        <div class="col text-center">
          <q-img src="statics/logo.png" style="width: 500px" />
        </div>

        <div class="col" style="height: 10px" />

        <div class="col">
          <q-toolbar class="rounded-borders" style="background-color: rgba(0, 0, 0, 0.5)">
            <q-tabs v-model="tab" inline-label class="text-white">
              <q-route-tab name="home" to="/" :label="langHome" exact />
              <q-route-tab name="news" to="/news" :label="langNews" exact />
              <q-route-tab name="forum" to="/forum" :label="langForum" exact />
              <q-route-tab name="shop" to="/shop" :label="langShop" exact />
              <q-route-tab name="download" to="/download" :label="langDownload" exact />
              <q-route-tab name="about" to="/about" :label="langAbout" exact />
            </q-tabs>

            <q-space />

            <q-chip v-show="logined">
              <q-avatar>
                <img src="statics/1.jpeg" />
              </q-avatar>User Avatar
            </q-chip>
          </q-toolbar>
        </div>
      </div>

      <q-page-container>
        <router-view style="min-height: 0; margin-top: 10px; margin-bottom: 10px" />
      </q-page-container>

      <div>
        <div class="rounded-borders" style="background-color: rgba(0, 0, 0, 0.5)">
          <div class="text-center text-white">Copyright (C) 2019 CaNplay</div>

          <div class="text-center">
            <q-btn
              flat
              text-color="info"
              icon="fab fa-twitter-square"
              @click="onSocial('twitter')"
            />
            <q-btn
              flat
              text-color="primary"
              icon="fab fa-facebook-square"
              @click="onSocial('facebook')"
            />
            <q-btn
              flat
              text-color="light-blue-14"
              icon="fab fa-discord"
              @click="onSocial('discord')"
            />
            <q-btn flat text-color="warning" icon="fab fa-weibo" @click="onSocial('weibo')" />
            <q-btn flat text-color="positive" icon="fab fa-weixin" @click="onSocial('weixin')" />
          </div>
        </div>
      </div>
    </div>
  </q-layout>
</template>

<script>
import config from "../components/config.js";

export default {
  name: "MyLayout",

  data() {
    return {
      tab: "home",
      areaHeight: "",
      logined: false,
      langHome: "",
      langNews: "",
      langForum: "",
      langShop: "",
      langDownload: "",
      langAbout: ""
    };
  },

  methods: {
    onSocial(type) {
      switch (type) {
        case "twitter":
          break;
        case "facebook":
          break;
        case "discord":
          break;
        case "weibo":
          break;
        case "weixin":
          break;
      }
    }
  },

  created() {
    this.langHome = config.lang[0]["主页"];
    this.langNews = config.lang[0]["新闻"];
    this.langForum = config.lang[0]["社区"];
    this.langShop = config.lang[0]["商城"];
    this.langDownload = config.lang[0]["下载"];
    this.langAbout = config.lang[0]["关于"];
  }
};
</script>
