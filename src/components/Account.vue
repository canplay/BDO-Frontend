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
      <div class="col-4 relative-position">
        <div class="absolute-center text-h4">06X2</div>
      </div>

      <div class="col">
        <q-input
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
      labelBtn: ""
    };
  },

  created() {
    this.labelBtn = this.mode;
  }
};
</script>

<style>
</style>