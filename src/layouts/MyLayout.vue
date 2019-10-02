<template>
  <q-layout>
    <q-img src="statics/bg.png" class="fixed-center fit" style="z-index: -1" />

    <div class="column">
      <div class="col" style="height: 10px" />

      <div class="col text-center" style="padding-left: 10px; padding-right: 10px">
        <q-img src="statics/logo.png" style="width: 400px" />
      </div>

      <div class="col" style="height: 10px" />

      <div class="col" style="padding-left: 10px; padding-right: 10px">
        <q-toolbar class="rounded-borders" style="background-color: rgba(0, 0, 0, 0.5)">
          <q-tabs v-model="tab" inline-label class="text-white">
            <q-route-tab name="home" to="/" label="Home" exact />
            <q-route-tab name="news" to="/news" label="News" exact />
            <q-route-tab name="forum" to="/forum" label="Forum" exact />
            <q-route-tab name="shop" to="/shop" label="Shop" exact />
            <q-route-tab name="download" to="/download" label="Download" exact />
            <q-route-tab name="about" to="/about" label="About" exact />
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
      <router-view />
    </q-page-container>

    <div style="padding-left: 10px; padding-right: 10px">
      <div class="rounded-borders" style="background-color: rgba(0, 0, 0, 0.5)">
        <div class="text-center text-white">Copyright (C) 2019 CaNplay</div>

        <div class="text-center">
          <q-btn flat text-color="info" icon="fab fa-twitter-square" @click="onSocial('twitter')" />
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
  </q-layout>
</template>

<script>
export default {
  name: "MyLayout",

  data() {
    return {
      tab: "home",
      areaHeight: "",
      logined: false
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
  }
};
</script>
