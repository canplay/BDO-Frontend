<template>
  <div>
    <editor
      v-model="editorText"
      style="min-height: 500px"
      @change="onEditorChange"
    />
  </div>
</template>

<script>
import "tui-editor/dist/tui-editor.css";
import "tui-editor/dist/tui-editor-contents.css";
import "codemirror/lib/codemirror.css";

export default {
  name: "PageAdminMarkdown",

  props: {
    database: String,
    type: String
  },

  components: {
    editor: () => import("@toast-ui/vue-editor/src/Editor.vue")
  },

  data() {
    return {
      editorText: null
    };
  },

  methods: {
    onEditorChange() {
      this.$axios.post(this.$store.state.custom.ip + "/overview", {
        token: this.$q.sessionStorage.getItem("token"),
        methond: this.type,
        markdown: this.editorText
      });
    }
  },

  created() {
    this.editorText = this.database;
  }
};
</script>

<style></style>
