<template>
  <div
    class="rounded-borders"
    style="background-color: rgba(0, 0, 0, 0.5); padding: 10px"
  >
    <q-splitter
      v-model="splitterModel"
      class="rounded-borders"
      style="background-color: rgba(0, 0, 0, 0.5)"
    >
      <template v-slot:before>
        <q-tabs v-model="tabModel" vertical class="text-white">
          <q-tab name="info" icon="mail" label="概览" />
          <q-tab name="news" icon="alarm" label="新闻页面" />
          <q-tab name="forum" icon="alarm" label="社区页面" />
          <q-tab name="shop" icon="alarm" label="商城页面" />
          <q-tab name="download" icon="alarm" label="下载页面" />
          <q-tab name="about" icon="alarm" label="关于页面" />
          <q-tab name="account" icon="movie" label="账号管理" />
        </q-tabs>
      </template>

      <template v-slot:after>
        <q-tab-panels
          class="transparent text-white"
          v-model="tabModel"
          animated
          transition-prev="jump-up"
          transition-next="jump-up"
        >
          <q-tab-panel name="info">
            <div class="text-h5">服务器时间：{{ os.time }}</div>
            <div class="text-h5">运行时长：{{ os.runtime }}</div>
            <div class="text-h5">启动器目录：{{ launcher }}</div>
            <div class="text-h5">版本：{{ update.version }}</div>
            <div class="text-h5">更新器目录：{{ update.path }}</div>
          </q-tab-panel>

          <q-tab-panel name="news"></q-tab-panel>

          <q-tab-panel name="forum"></q-tab-panel>

          <q-tab-panel name="shop"></q-tab-panel>

          <q-tab-panel name="download"></q-tab-panel>

          <q-tab-panel name="about"></q-tab-panel>

          <q-tab-panel name="account"></q-tab-panel>
        </q-tab-panels>
      </template>
    </q-splitter>
  </div>
</template>

<script>
export default {
  name: "PageAdmin",

  data() {
    return {
      splitterModel: 10,
      tabModel: "info",
      os: {
        time: 0,
        runtime: 0
      },
      launcher: "",
      update: {
        version: "",
        path: ""
      }
    };
  }
};
</script>

<style></style>
