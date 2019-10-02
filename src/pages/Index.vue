<template>
  <q-page style="padding: 10px">
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
            <q-btn label="Register" color="primary" @click="onRegister" class="fit" />
          </div>

          <div class="col-1" />

          <div class="col">
            <q-btn label="Login" color="negative" @click="onLogin" class="fit" />
          </div>
        </div>
      </div>

      <div class="col" style="height: 10px" />

      <div class="col row">
        <div class="col">
          <q-card dark style="background-color: rgba(0, 0, 0, 0.5)">
            <q-card-section>
              <div class="text-h6">Notice</div>
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
              <div class="text-h6">News</div>
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
          <div class="text-h6">{{ dialogAccount.mode }}</div>
        </q-card-section>

        <q-card-section>
          <account :mode="dialogAccount.mode" />
        </q-card-section>
      </q-card>
    </q-dialog>
  </q-page>
</template>

<script>
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
      }
    };
  },

  methods: {
    onRegister() {
      this.dialogAccount.mode = "Register";
      this.dialogAccount.show = true;
    },

    onLogin() {
      this.dialogAccount.mode = "Login";
      this.dialogAccount.show = true;
    }
  },

  created() {
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
