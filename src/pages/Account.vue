<template>
  <div class="colmun">
    <div class="col">
      <q-input
        standout="bg-teal text-white"
        v-model="username"
        label="Username"
        bg-color="white"
      />
    </div>

    <div class="col-1" style="height: 20px" />

    <div class="col">
      <q-input
        standout="bg-teal text-white"
        v-model="password"
        label="Password"
        bg-color="white"
      />
    </div>

    <div class="col-1" style="height: 20px" />

    <div class="col row">
      <div class="col-4 relative-position" ref="captchaCotainer">
        <img ref="imgCaptcha" @click="onCaptcha" />
        <canvas ref="canvas" style="display: none" />
      </div>

      <div class="col-1" />

      <div class="col">
        <q-input
          ref="captchaInput"
          standout="bg-teal text-white"
          v-model="captcha"
          label="Captcha"
          bg-color="white"
        />
      </div>
    </div>

    <div class="col-1" style="height: 20px" />

    <div class="col">
      <q-btn :label="labelBtn" color="primary" class="fit" @click="onBtn" />
    </div>
  </div>
</template>

<script>
import captcha from "../components/captcha.js";
import config from "../components/config.js";

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
    onResize() {
      this.$refs.canvas.height = this.$refs.captchaInput.$el.clientHeight;
      this.$refs.canvas.width = this.$refs.captchaCotainer.clientWidth;
    },

    onCaptcha() {
      this.imgCaptcha = captcha.drawCode(
        this.$refs.canvas,
        this.$refs.imgCaptcha
      );
    },

    onBtn() {
      if (!this.username || this.username == "") {
        this.$q.notify(this.$t("请输入用户名"));
        return;
      }

      if (this.username.length < 3) {
        this.$q.notify(this.$t("用户名不能小于3个字"));
        return;
      }

      if (!this.password || this.password == "") {
        this.$q.notify(this.$t("请输入密码"));
        return;
      }

      if (this.password.length < 6) {
        this.$q.notify(this.$t("密码不能小于6个字"));
        return;
      }

      if (!this.captcha || this.captcha == "") {
        this.$q.notify(this.$t("请输入验证码"));
        return;
      }

      if (this.captcha != this.imgCaptcha) {
        this.$q.notify(this.$t("验证码错误"));
        return;
      }

      let regex = /^[0-9a-zA-Z]+$/;
      if (!regex.test(this.username) || !regex.test(this.password)) {
        this.$q.notify(this.$t("用户名或密码只能是字母和数字"));
        return;
      }

      this.$q.loading.show();

      let timer = window.setTimeout(() => {
        this.$q.loading.hide();
        this.$q.notify(this.$t("网络连接失败"));
        window.clearTimeout(timer);
      }, 60000);

      switch (this.mode) {
        case "Register":
          this.$axios
            .get(config.ip + "/register/" + this.username + "/" + this.password)
            .then(response => {
              switch (response.data) {
                case "id error":
                  this.$q.loading.hide();
                  window.clearTimeout(timer);
                  this.$q.notify(this.$t("注册超时"));
                  break;
                case "captcha error":
                  this.$q.loading.hide();
                  window.clearTimeout(timer);
                  this.$q.notify(this.$t("验证码错误"));
                  break;
                case "username exist":
                  this.$q.loading.hide();
                  window.clearTimeout(timer);
                  this.$q.notify(this.$t("账号已存在"));
                  break;
                case "success":
                  this.$emit("event", "registered");
                  this.$q.loading.hide();
                  window.clearTimeout(timer);
                  this.$q.notify(this.$t("注册成功"));
                  break;
                default:
                  this.$q.loading.hide();
                  window.clearTimeout(timer);
                  this.$q.notify(this.$t("注册失败"));
                  break;
              }
            });
          break;
        case "Login":
          this.$axios
            .get(config.ip + "/login/" + this.username + "/" + this.password)
            .then(response => {
              switch (response.data) {
                case "username error":
                  this.$q.loading.hide();
                  window.clearTimeout(timer);
                  this.$q.notify(this.$t("用户名错误"));
                  break;
                case "password error":
                  this.$q.loading.hide();
                  window.clearTimeout(timer);
                  this.$q.notify(this.$t("密码错误"));
                  break;
                case "success":
                  this.$emit("event", "login", this.username, this.password);
                  this.$q.loading.hide();
                  window.clearTimeout(timer);
                  break;
                default:
                  this.$q.loading.hide();
                  window.clearTimeout(timer);
                  this.$q.notify(this.$t("登录失败！"));
                  break;
              }
            });
          break;
      }
    }
  },

  created() {
    window.addEventListener("resize", this.onResize);

    switch (this.mode) {
      case "Register":
        this.labelBtn = this.$t("注册");
        break;
      case "Login":
        this.labelBtn = this.$t("登录");
        break;
    }
  },

  mounted() {
    this.onResize();
    this.onCaptcha();
  },

  destroyed() {
    window.removeEventListener("resize", this.onResize);
  }
};
</script>

<style></style>
