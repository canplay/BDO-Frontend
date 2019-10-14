<template>
  <q-layout>
    <q-img src="statics/bg.png" class="fixed-center fit" style="z-index: -1" />

    <div
      class="fixed-center fit"
      style="min-width: 950px; padding-left: 150px; padding-right: 150px; padding-top: 50px"
    >
      <div class="column">
        <div class="col" style="height: 10px" />

        <div class="col text-center">
          <q-img src="statics/logo.png" style="width: 500px" />
        </div>

        <div class="col" style="height: 10px" />

        <div class="col">
          <q-toolbar
            class="rounded-borders"
            style="background-color: rgba(0, 0, 0, 0.5)"
          >
            <q-tabs v-model="tab" inline-label class="text-white">
              <q-route-tab name="home" to="/" :label="$t('主页')" exact />
              <q-tab name="forum" :label="$t('社区')" @click="onForum" />
              <q-route-tab name="shop" to="/shop" :label="$t('商城')" exact />
              <q-route-tab
                name="download"
                to="/download"
                :label="$t('下载')"
                exact
              />
              <q-route-tab name="about" to="/about" :label="$t('关于')" exact />
              <q-route-tab
                v-if="this.$store.state.custom.logined"
                name="admin"
                to="/admin"
                :label="$t('管理')"
                exact
              />
            </q-tabs>

            <q-space />

            <!-- <q-chip v-show="logined">
              <q-avatar>
                <img src="statics/1.jpeg" />
              </q-avatar>
              User Avatar
            </q-chip>

            <q-space />-->

            <q-btn-dropdown stretch flat :label="$t('语言')" class="text-white">
              <q-list>
                <q-item clickable v-close-popup @click="onChangeLang(0)">
                  <q-item-section>
                    <q-item-label>简体中文</q-item-label>
                  </q-item-section>
                </q-item>

                <q-item clickable v-close-popup @click="onChangeLang(1)">
                  <q-item-section>
                    <q-item-label>English</q-item-label>
                  </q-item-section>
                </q-item>
              </q-list>
            </q-btn-dropdown>
          </q-toolbar>
        </div>
      </div>

      <q-page-container>
        <router-view
          style="min-height: 0; margin-top: 10px; margin-bottom: 10px"
        />
      </q-page-container>

      <div>
        <div
          class="rounded-borders"
          style="background-color: rgba(0, 0, 0, 0.5)"
        >
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
            <q-btn
              flat
              text-color="warning"
              icon="fab fa-weibo"
              @click="onSocial('weibo')"
            />
            <q-btn
              flat
              text-color="positive"
              icon="fab fa-weixin"
              @click="onSocial('weixin')"
            />
          </div>
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
      weblink: {
        forum: null,
        twitter: null,
        facebook: null,
        discord: null,
        weibo: null,
        weixin: null
      }
    };
  },

  methods: {
    onChangeLang(index) {
      switch (index) {
        case 0:
          this.$i18n.locale = "zh-cn";
          break;
        case 1:
          this.$i18n.locale = "en-us";
          break;
      }
    },

    onSocial(type) {
      switch (type) {
        case "twitter":
          window.open(this.weblink.twitter, "_blank");
          break;
        case "facebook":
          window.open(this.weblink.facebook, "_blank");
          break;
        case "discord":
          window.open(this.weblink.discord, "_blank");
          break;
        case "weibo":
          window.open(this.weblink.weibo, "_blank");
          break;
        case "weixin":
          window.open(this.weblink.weixin, "_blank");
          break;
      }
    },

    onForum() {
      window.open(this.weblink.forum, "_blank");
    }
  },

  created() {
    this.$i18n.locale = "zh-cn";

    this.$axios
      .get(this.$store.state.custom.ip + "/home/config")
      .then(response => {
        this.weblink.forum = response.data.msg[0].forum;
        this.weblink.twitter = response.data.msg[0].twitter;
        this.weblink.facebook = response.data.msg[0].facebook;
        this.weblink.discord = response.data.msg[0].discord;
        this.weblink.weibo = response.data.msg[0].weibo;
        this.weblink.weixin = response.data.msg[0].weixin;
      });

    let token = this.$q.sessionStorage.getItem("token");
    if (token) {
      this.$store.commit("custom/login", true);
    }
  }
};
</script>
