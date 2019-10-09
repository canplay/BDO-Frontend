<template>
  <div
    class="rounded-borders"
    style="background-color: rgba(0, 0, 0, 0.5); padding: 10px"
  >
    <viewer :value="viewerText" />
  </div>
</template>

<script>
import "tui-editor/dist/tui-editor-contents.css";
import "highlight.js/styles/github.css";
import Viewer from "@toast-ui/vue-editor/src/Viewer.vue";

export default {
  name: "PageView",

  components: {
    viewer: Viewer
  },

  data() {
    return {
      viewerText: ""
    };
  },

  created() {
    this.$axios
      .get(this.$store.state.custom.ip + "/news/" + this.$route.query.id)
      .then(response => {
        this.viewerText = response.data.msg.content;
      });
  }
};
</script>

<style></style>
