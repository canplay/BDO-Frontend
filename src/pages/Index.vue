<template>
  <q-page>
    <q-card style="background-color: rgba(0, 0, 0, 0.5)">
      <q-card-section>
        <div class="q-gutter-md">
          <q-input
            standout="bg-teal text-white"
            v-model="username"
            label="用户名"
            dark
            bg-color="black"
            style="opacity: 0.5"
          />
          <q-input
            standout="bg-teal text-white"
            v-model="password"
            label="密码"
            dark
            bg-color="black"
            style="opacity: 0.5"
          />

          <q-btn label="登录" color="primary" class="fit" @click="onLogin" />
        </div>
      </q-card-section>
    </q-card>
  </q-page>
</template>

<script>
export default {
  name: "PageIndex"
};
</script>
