<template>
  <q-page>
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
            <q-btn :label="langRegister" color="primary" @click="onRegister" class="fit" />
          </div>

          <div class="col-1" />

          <div class="col">
            <q-btn :label="langLogin" color="negative" @click="onLogin" class="fit" />
          </div>
        </div>
      </div>

      <div class="col" style="height: 10px" />

      <div class="col row fit">
        <div class="col">
          <q-card dark style="background-color: rgba(0, 0, 0, 0.5)">
            <q-card-section>
              <div class="text-h6">{{ langNotice }}</div>
            </q-card-section>

            <q-separator dark inset />

            <q-card-section>
              <q-list dark>
                <q-item clickable v-ripple v-for="(item, index) in News.Notice" :key="index">
                  <q-item-section>{{ item.title }}</q-item-section>
                </q-item>
              </q-list>
            </q-card-section>
          </q-card>
        </div>

        <div class="col-1" style="width: 1%" />

        <div class="col">
          <q-card dark style="background-color: rgba(0, 0, 0, 0.5)">
            <q-card-section>
              <div class="text-h6">{{ langNews }}</div>
            </q-card-section>

            <q-separator dark inset />

            <q-card-section>
              <q-list dark>
                <q-item clickable v-ripple v-for="(item, index) in News.News" :key="index">
                  <q-item-section>{{ item.title }}</q-item-section>
                </q-item>
              </q-list>
            </q-card-section>
          </q-card>
        </div>
      </div>
    </div>

    <q-dialog v-model="dialogAccount.show">
      <q-card class="text-white" style="background-color: rgba(0, 0, 0, 0.5); width: 50%">
        <q-card-section>
          <div class="text-h6 text-center">{{ langDlgAccount }}</div>
        </q-card-section>

        <q-card-section>
          <account :mode="dialogAccount.mode" />
        </q-card-section>
      </q-card>
    </q-dialog>
  </q-page>
</template>

<script>
import config from "../components/config.js";

export default {
  name: "PageIndex",

  components: {
    account: () => import("components/Account.vue")
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
      langNotice: "",
      langNews: "",
      langDlgAccount: "",
      langRegister: "",
      langLogin: ""
    };
  },

  methods: {
    onRegister() {
      this.dialogAccount.mode = "Register";
      this.langDlgAccount = config.lang[0]["注册"];
      this.dialogAccount.show = true;
    },

    onLogin() {
      this.dialogAccount.mode = "Login";
      this.langDlgAccount = config.lang[0]["登录"];
      this.dialogAccount.show = true;
    }
  },

  created() {
    this.langNotice = config.lang[0]["公告"];
    this.langNews = config.lang[0]["新闻"];
    this.langRegister = config.lang[0]["注册"];
    this.langLogin = config.lang[0]["登录"];

    this.slide.list.push({ title: "Slide 1", src: "statics/1.jpeg" });
    this.slide.list.push({ title: "Slide 2", src: "statics/2.jpg" });
    this.slide.list.push({ title: "Slide 3", src: "statics/3.png" });
    this.slide.model = "Slide 1";

    for (let index = 0; index < 10; index++) {
      this.News.Notice.push({ title: "Notice " + index });
      this.News.News.push({ title: "News " + index });
    }
  }
};
</script>
