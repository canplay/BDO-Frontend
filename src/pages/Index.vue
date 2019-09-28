<template>
  <q-page :style="{ 'min-height': minHeight + 'px', height: minHeight + 'px' }">
    <div class="column vertical-middle">
      <div class="col-auto text-center">
        <q-img src="statics/logo.png" style="width: 500px; height: 111px" />
      </div>

      <div class="col-1" style="margin: 1%" />

      <div class="col fixed-center" style="width: 50%">
        <q-card style="background-color: rgba(0, 0, 0, 0.5)">
          <q-card-section>
            <div class="q-gutter-md">
              <q-input
                standout="bg-teal text-white"
                v-model="username"
                :label="label_Username"
                dark
                bg-color="black"
                style="opacity: 0.5"
              />
              <q-input
                standout="bg-teal text-white"
                v-model="password"
                :label="label_Password"
                dark
                bg-color="black"
                style="opacity: 0.5"
              />

              <q-btn
                :label="label_Login"
                color="primary"
                class="fit"
                @click="onLogin"
              />
            </div>
          </q-card-section>
        </q-card>
      </div>
    </div>
  </q-page>
</template>

<style></style>

<script>
import common from "../store/common.js";

export default {
  name: "PageIndex",

  data() {
    return {
      username: "",
      password: "",
      minHeight: 0,
      lang: "",
      label_Username: "",
      label_Password: "",
      label_Login: ""
    };
  },

  methods: {
    onResize() {
      this.minHeight = window.innerHeight - 50;
    },

    openurl(url) {
      common.ipc("newWindow", url, "webview");
    },

    login() {},

    onLogin() {
      if (!this.username || this.username == "") {
        this.$q.notify(common.lang["请输入用户名"]);
        return;
      }

      if (!this.password || this.password == "") {
        this.$q.notify(common.lang["请输入密码"]);
        return;
      }

      common.RequestURL(
        "http://127.0.0.1:3000/login/" + this.username + "/" + this.password,
        "",
        "",
        "GET",
        data => {
          console.log(data);
          switch (data) {
            case "success":
              this.$q.notify("登陆成功！");
              break;
            case "failed":
              this.$q.notify("登陆失败！");
              break;
            case "username error":
              this.$q.notify("用户名错误！");
              break;
            default:
              this.$q.notify("未知错误！");
              break;
          }
        }
      );
    }
  },

  created() {
    window.addEventListener("resize", this.onResize);

    common.applyLoc();

    this.label_Username = common.lang["用户名"];
    this.label_Password = common.lang["密码"];
    this.label_Login = common.lang["登录"];
  },

  mounted() {
    this.onResize();
  },

  destroyed() {
    window.removeEventListener("resize", this.onResize);
  }
};
</script>
