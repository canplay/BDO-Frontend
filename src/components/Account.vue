<template>
  <div class="colmun">
    <div class="col">
      <q-input standout="bg-teal text-white" v-model="username" label="Username" bg-color="white" />
    </div>

    <div class="col-1" style="height: 20px" />

    <div class="col">
      <q-input standout="bg-teal text-white" v-model="password" label="Password" bg-color="white" />
    </div>

    <div class="col-1" style="height: 20px" />

    <div class="col row">
      <div class="col-4 relative-position">
        <div class="absolute-center text-h4">{{ imgCaptcha }}</div>
      </div>

      <div class="col">
        <q-input standout="bg-teal text-white" v-model="captcha" label="Captcha" bg-color="white" />
      </div>
    </div>

    <div class="col-1" style="height: 20px" />

    <div class="col">
      <q-btn :label="labelBtn" color="primary" class="fit" @click="onBtn" />
    </div>
  </div>
</template>

<script>
import config from "../components/config.js";
import common from "../components/common.js";

export default {
  name: "PageAccount",

  props: {
    mode: String
  },

  data() {
    return {
      username: "",
      password: "",
      captcha: "",
      imgCaptcha: "",
      labelBtn: "",
      id: 0
    };
  },

  methods: {
    onBtn() {
      if (!this.username || this.username == "") {
        this.$q.notify(config.lang[0]["请输入用户名"]);
        return;
      }

      if (this.username.length < 3) {
        this.$q.notify(config.lang[0]["用户名不能小于3个字"]);
        return;
      }

      if (!this.password || this.password == "") {
        this.$q.notify(config.lang[0]["请输入密码"]);
        return;
      }

      if (this.password.length < 6) {
        this.$q.notify(config.lang[0]["密码不能小于6个字"]);
        return;
      }

      if (!this.captcha || this.captcha == "") {
        this.$q.notify(config.lang[0]["请输入验证码"]);
        return;
      }

      let regex = /^[0-9a-zA-Z]+$/;
      if (!regex.test(this.username) || !regex.test(this.password)) {
        this.$q.notify(config.lang[0]["用户名或密码只能是字母和数字"]);
        return;
      }

      switch (this.mode) {
        case "Register":
          this.$q.loading.show();

          let timer = window.setTimeout(() => {
            this.$q.loading.hide();
            this.$q.notify(config.lang[0]["注册失败"]);
            window.clearTimeout(timer);
          }, 180000);

          common.RequestURL(
            config.ip +
              "register/" +
              this.id +
              "/" +
              this.captcha +
              "/" +
              this.username +
              "/" +
              this.password,
            "",
            "GET",
            (status, data) => {
              if (status == "success") {
                switch (data) {
                  case "id error":
                    this.$q.loading.hide();
                    window.clearTimeout(timer);
                    this.$q.notify(
                      config.lang[0]["注册超时，请重新打开注册窗口"]
                    );
                    break;
                  case "captcha error":
                    this.$q.loading.hide();
                    window.clearTimeout(timer);
                    this.$q.notify(config.lang[0]["验证码错误，请重新填写"]);
                    break;
                  case "username exist":
                    this.$q.loading.hide();
                    window.clearTimeout(timer);
                    this.$q.notify(config.lang[0]["账号已存在，请重新填写"]);
                    break;
                  case "success":
                    this.$q.loading.hide();
                    window.clearTimeout(timer);
                    this.$q.notify(config.lang[0]["注册成功！"]);
                    break;
                  default:
                    this.$q.loading.hide();
                    window.clearTimeout(timer);
                    this.$q.notify(config.lang[0]["注册失败！"]);
                    break;
                }
              }
            }
          );
          break;
      }
    }
  },

  created() {
    this.labelBtn = this.mode;

    common.RequestURL(config.ip + "captcha", "", "GET", (status, data) => {
      if (status === "success") {
        let json = JSON.parse(data);
        this.id = json.id;
        this.imgCaptcha = json.captcha;
      } else {
        this.$q.notify(config.lang[0]["验证码加载失败，请重新打开注册窗口"]);
      }
    });
  }
};
</script>

<style>
</style>