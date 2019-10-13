<template>
  <div>
    <q-table
      :data="data"
      :columns="columns"
      row-key="title"
      :selected-rows-label="getSelectedString"
      selection="multiple"
      :selected.sync="selected"
    >
      <template v-slot:top="props">
        <div class="col-3 q-table__title">{{ $t("滚动") }}</div>

        <q-space />

        <div class="col-auto q-gutter-xs">
          <q-btn color="primary" :label="$t('插入')" />
          <q-btn color="primary" :label="$t('删除')" />
        </div>
      </template>
    </q-table>

    <div>Selected: {{ JSON.stringify(selected) }}</div>

    <br />

    <q-table
      :data="data"
      :columns="columns"
      row-key="title"
      :selected-rows-label="getSelectedString"
      selection="multiple"
      :selected.sync="selected"
    >
      <template v-slot:top="props">
        <div class="col-3 q-table__title">{{ $t("公告") }}</div>

        <q-space />

        <div class="col-auto q-gutter-xs">
          <q-btn color="primary" :label="$t('插入')" />
          <q-btn color="primary" :label="$t('删除')" />
        </div>
      </template>
    </q-table>

    <br />

    <q-table
      :data="data"
      :columns="columns"
      row-key="title"
      :selected-rows-label="getSelectedString"
      selection="multiple"
      :selected.sync="selected"
    >
      <template v-slot:top="props">
        <div class="col-3 q-table__title">{{ $t("新闻") }}</div>

        <q-space />

        <div class="col-auto q-gutter-xs">
          <q-btn color="primary" :label="$t('插入')" />
          <q-btn color="primary" :label="$t('删除')" />
        </div>
      </template>
    </q-table>
  </div>
</template>

<script>
export default {
  name: "PageAdminNews",

  props: {
    database: String
  },

  data() {
    return {
      selected: [],
      data: [
        {
          title: "test",
          img: "",
          href: ""
        }
      ],
      columns: [
        {
          name: "title",
          align: "left",
          label: this.$t("标题"),
          field: "title"
        },
        {
          name: "img",
          align: "left",
          label: this.$t("图片"),
          field: "img"
        },
        {
          name: "href",
          align: "left",
          label: this.$t("链接"),
          field: "href"
        }
      ]
    };
  },

  methods: {
    getSelectedString() {
      return this.selected.length === 0
        ? ""
        : `${this.selected.length} record${
            this.selected.length > 1 ? "s" : ""
          } selected of ${this.data.length}`;
    }
  }
};
</script>

<style></style>
