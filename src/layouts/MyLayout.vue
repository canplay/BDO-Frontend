<template>
  <q-layout>
    <q-img
      src="statics/bg.png"
      class="fixed-center fit"
      style="z-index: -1; border-radius: 8px"
    />

    <q-toolbar class="text-white q-electron-drag transprent">
      <q-img
        src="statics/icon.png"
        style="width: 36px; height: 36px"
      />&nbsp;&nbsp;
      <div class="text-black">
        <q-badge class="bg-accent">{{ title }} {{ version }}</q-badge>
      </div>

      <q-space />

      <q-btn
        v-if="appurl === 'index'"
        dense
        flat
        color="black"
        icon="file_download"
        @click="download"
      >
        <q-tooltip v-model="show_download" content-style="font-size: 12px">{{
          download_tip
        }}</q-tooltip>
      </q-btn>

      <q-btn
        v-if="appurl === 'index'"
        dense
        flat
        color="black"
        icon="settings"
        @click="option"
      >
        <q-tooltip content-style="font-size: 12px">
          {{ label_Option }}
        </q-tooltip>
      </q-btn>

      <q-btn dense flat color="black" icon="minimize" @click="minimize">
        <q-tooltip content-style="font-size: 12px">
          {{ label_Minimize }}
        </q-tooltip>
      </q-btn>

      <q-btn dense flat color="black" icon="crop_square" @click="maximize">
        <q-tooltip content-style="font-size: 12px">
          {{ label_Maximize }}
        </q-tooltip>
      </q-btn>

      <q-btn dense flat color="black" icon="close" @click="close">
        <q-tooltip content-style="font-size: 12px">{{ label_Close }}</q-tooltip>
      </q-btn>
    </q-toolbar>

    <q-page-container>
      <router-view
        transition-show="fade"
        transition-hide="fade"
        ref="container"
        :style="{ 'min-height': minHeight + 'px', height: minHeight + 'px' }"
      />
    </q-page-container>

    <q-dialog v-model="dlg_option">
      <q-card style="width: 600px">
        <q-card-section class="fit items-center">
          <div class="q-gutter-md">
            <div class="row">
              <div class="col">
                <q-btn
                  class="fit"
                  :label="label_btnSave"
                  color="primary"
                  @click="onSave"
                />
              </div>

              <div class="col-1" />

              <div class="col">
                <q-btn
                  class="fit"
                  :label="label_btnReset"
                  color="primary"
                  flat
                  @click="onReset"
                />
              </div>
            </div>
          </div>
        </q-card-section>
      </q-card>
    </q-dialog>

    <q-dialog v-model="dlg_alert">
      <q-card>
        <q-card-section>
          <div class="text-h6">Notice</div>
        </q-card-section>

        <q-card-section>{{ dlg_alertInfo }}</q-card-section>

        <q-card-actions align="right">
          <q-btn flat :label="label_ok" color="primary" v-close-popup />
        </q-card-actions>
      </q-card>
    </q-dialog>
  </q-layout>
</template>

<style>
* {
  -webkit-user-select: none;
  -webkit-touch-callout: none;
}
body::-webkit-scrollbar {
  width: 10px;
  height: 10px;
}
body::-webkit-scrollbar-track {
  box-shadow: inset 0 0 5px rgba(0, 0, 0, 0.2);
  -webkit-box-shadow: inset 0 0 5px rgba(0, 0, 0, 0.2);
  border-radius: 10px;
  background: #ededed;
}
body::-webkit-scrollbar-thumb {
  border-radius: 10px;
  box-shadow: inset 0 0 5px rgba(0, 0, 0, 0.2);
  -webkit-box-shadow: inset 0 0 5px rgba(0, 0, 0, 0.2);
  background: #750050;
}
</style>

<script>
import common from "../store/common.js";

export default {
  name: "MyLayout",

  data() {
    return {
      title: this.$q.electron.remote.getCurrentWindow().getTitle(),
      appurl: "index",
      style: { width: "", height: "" },
      dlg_option: false,
      dlg_alert: false,
      dlg_alertInfo: "",
      download_tip: "",
      show_download: false,
      version: "0.0.0",
      label_btnSave: "",
      label_btnReset: "",
      label_ok: "",
      label_Option: "",
      label_Minimize: "",
      label_Maximize: "",
      label_Close: "",
      maximized: false,
      minHeight: 0
    };
  },

  methods: {
    minimize() {
      this.$q.electron.remote.getCurrentWindow().minimize();
    },

    maximize() {
      if (this.maximized) {
        this.$q.electron.remote.getCurrentWindow().unmaximize();
        this.maximized = false;
      } else {
        this.$q.electron.remote.getCurrentWindow().maximize();
        this.maximized = true;
      }
    },

    close() {
      window.close();
    },

    onResize() {
      this.style = {
        width: window.innerWidth + "px",
        height: window.innerHeight - 32 + "px"
      };
      this.minHeight = window.innerHeight - 50;
    },

    reload(url) {
      this.$router.push({
        path: "/redirect",
        query: url
      });
    },

    download() {
      common.ipc("newWindow", "./statics/AriaNg.html", "webview");
    },

    option() {
      this.dlg_option = true;
    },

    onSave() {
      this.$q.loading.show({
        message: "<b>" + common.lang["正在保存设置..."] + "</b>"
      });

      let timer = window.setTimeout(() => {
        this.$q.loading.hide();
      }, 180000);

      // if (this.autostart) common.EnableAutoStart();
      // else common.DisableAutoStart();

      this.$q.loading.hide();
      window.clearTimeout(timer);
    },

    onReset() {},

    alert(val) {
      this.dlg_alertInfo = val;
      this.dlg_alert = true;
    },

    download_start(arg) {
      this.download_tip = common.lang["正在下载"] + arg;
      this.show_download = true;

      self.setInterval(() => {
        this.download_tip = common.lang["下载"];
        this.show_download = false;
      }, 3000);
    },

    download_complete(arg) {
      this.download_tip = common.lang["下载完毕"] + arg;
      this.show_download = true;

      self.setInterval(() => {
        this.download_tip = common.lang["下载"];
        this.show_download = false;
      }, 3000);
    },

    showFileDlg() {
      this.$q.electron.remote.dialog
        .showOpenDialog({
          properties: ["openFile"],
          filters: [{ name: "BlackDesert64.exe", extensions: ["exe"] }]
        })
        .then(result => {
          this.dir = result.filePaths[0].replace(/\\/g, "/");
        });
    }
  },

  created() {
    window.addEventListener("resize", this.onResize);

    this.appurl = common.GetUrlParamValue("appurl");
    if (this.appurl != "index")
      this.$router.replace("window" + window.location.search);
    else this.$router.replace("index");

    // if (this.autostart) common.EnableAutoStart();
    // else common.DisableAutoStart();

    this.$root.$on("download_start", this.download_start);
    this.$root.$on("download_complete", this.download_complete);

    common.applyLoc();

    this.download_tip = common.lang["下载"];
    this.label_Dir = common.lang["游戏所在目录"];
    this.label_btnSave = common.lang["保存"];
    this.label_btnReset = common.lang["重置"];
    this.label_ok = common.lang["确定"];
    this.label_Remember = common.lang["保存密码"];
    this.label_AutoLogin = common.lang["自动登录"];
    this.label_Option = common.lang["设置"];
    this.label_Minimize = common.lang["最小化"];
    this.label_Maximize = common.lang["最大化"];
    this.label_Close = common.lang["关闭"];
  },

  destroyed() {
    window.removeEventListener("resize", this.onresize);
    this.$root.$off("download_start", this.download_start);
    this.$root.$off("download_complete", this.download_complete);
  },

  mounted() {
    this.onResize();
  }
};
</script>
