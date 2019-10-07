<template>
  <div class="column">
    <div class="col row">
      <div class="col-10">
        <q-carousel
          arrows
          animated
          infinite
          autoplay
          height="200px"
          class="text-white text-center"
          v-model="slide.model"
        >
          <q-carousel-slide
            v-for="(item, index) in slide.list"
            :key="index"
            :name="item.title"
            :img-src="item.src"
            @click="onSlide(item.href)"
          >
            <div class="absolute-bottom">
              <div class="text-h5">{{ item.title }}</div>
            </div>
          </q-carousel-slide>
        </q-carousel>
      </div>

      <div class="col-1" style="width: 2%" />

      <div class="col column">
        <div class="col">
          <q-btn
            :label="logined ? $t('启动') : $t('注册')"
            color="primary"
            @click="onRegister"
            class="fit"
          />
        </div>

        <div class="col-1" />

        <div class="col">
          <q-btn
            :label="logined ? $t('退出') : $t('登录')"
            color="negative"
            @click="onLogin"
            class="fit"
          />
        </div>
      </div>
    </div>

    <div class="col" style="height: 10px" />

    <div class="col row fit">
      <div class="col">
        <q-card dark style="background-color: rgba(0, 0, 0, 0.5)">
          <q-card-section>
            <div class="text-h6">{{ $t("公告") }}</div>
          </q-card-section>

          <q-separator dark inset />

          <q-card-section>
            <q-scroll-area style="height: 240px">
              <q-list dark>
                <q-item
                  clickable
                  v-ripple
                  v-for="(item, index) in News.Notice"
                  :key="index"
                  @click="onNotice(item.id, item.href)"
                >
                  <q-item-section>{{ item.title }}</q-item-section>
                </q-item>
              </q-list>
            </q-scroll-area>
          </q-card-section>
        </q-card>
      </div>

      <div class="col-1" style="width: 1%" />

      <div class="col">
        <q-card dark style="background-color: rgba(0, 0, 0, 0.5)">
          <q-card-section>
            <div class="text-h6">{{ $t("新闻") }}</div>
          </q-card-section>

          <q-separator dark inset />

          <q-card-section>
            <q-scroll-area style="height: 240px">
              <q-list dark>
                <q-item
                  clickable
                  v-ripple
                  v-for="(item, index) in News.News"
                  :key="index"
                  @click="onNews(item.id, item.href)"
                >
                  <q-item-section>{{ item.title }}</q-item-section>
                </q-item>
              </q-list>
            </q-scroll-area>
          </q-card-section>
        </q-card>
      </div>
    </div>

    <q-dialog v-model="dialogAccount.show">
      <q-card
        class="text-white"
        style="background-color: rgba(0, 0, 0, 0.5); width: 50%"
      >
        <q-card-section>
          <div class="text-h6 text-center">
            {{ dialogAccount.mode === "Register" ? $t("注册") : $t("登录") }}
          </div>
        </q-card-section>

        <q-card-section>
          <account @event="onEvent" :mode="dialogAccount.mode" />
        </q-card-section>
      </q-card>
    </q-dialog>
  </div>
</template>

<script>
import config from "../components/config.js";

export default {
  name: "PageIndex",

  components: {
    account: () => import("pages/Account.vue")
  },

  data() {
    return {
      slide: {
        model: "",
        list: []
      },
      dialogAccount: {
        show: false,
        mode: ""
      },
      News: {
        Notice: [],
        News: []
      },
      logined: false,
      username: "",
      password: ""
    };
  },

  methods: {
    onEvent(e, username, password) {
      switch (e) {
        case "registered":
          this.dialogAccount.show = false;
          break;
        case "login":
          this.username = username;
          this.password = password;
          this.dialogAccount.show = false;
          this.logined = true;
          this.$emit("event", "login");
          break;
      }
    },

    onRegister() {
      if (this.logined) {
        let server;
        this.$axios
          .get(config.ip + "/home/config")
          .then(response => {
            server = response.data[0].server;

            window.location.href =
              "BDOLauncher://" +
              server +
              "&" +
              this.username +
              "&" +
              this.password;
          })
          .catch(error => {
            this.$q.notify(error);
          });
      } else {
        this.dialogAccount.mode = "Register";
        this.dialogAccount.show = true;
      }
    },

    onLogin() {
      if (this.logined) {
        this.logined = false;
        this.$emit("event", "logout");
      } else {
        this.dialogAccount.mode = "Login";
        this.dialogAccount.show = true;
      }
    },

    onSlide(href) {
      window.open(href, "_blank");
    },

    onNotice(index, href) {
      if (href && href != "") {
        window.open(href, "_blank");
        return;
      }
      this.$router.push({ path: "View", query: { id: index } });
    },

    onNews(index, href) {
      if (href && href != "") {
        window.open(href, "_blank");
        return;
      }
      this.$router.push({ path: "View", query: { id: index } });
    }
  },

  created() {
    this.slide.list = [];
    this.$axios.get(config.ip + "/home/slide").then(response => {
      response.data.forEach(element => {
        this.slide.list.push({
          title: element.title,
          src: element.img,
          href: element.href
        });
      });
      this.slide.model = response.data[0].title;
    });

    this.News.Notice = [];
    this.News.News = [];
    this.$axios.get(config.ip + "/home/news").then(response => {
      response.data.forEach(element => {
        if (element.type === "notice") {
          this.News.Notice.push({
            id: element._id,
            title: element.title,
            href: element.href,
            content: element.content
          });
        } else {
          this.News.News.push({
            id: element._id,
            title: element.title,
            href: element.href,
            content: element.content
          });
        }
      });
    });
  }
};
</script>
