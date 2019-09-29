<template>
  <q-page padding>
    <q-splitter
      v-model="splitterModel"
      class="fit"
      style="background-color: rgba(0, 0, 0, 0.8)"
      :limits="[10, 10]"
    >
      <template v-slot:before>
        <q-tabs v-model="tab" vertical class="text-teal" style="witdh: 20%">
          <q-tab name="dashboard" icon="dashboard" label="概览" />
          <q-tab name="account" icon="account_box" label="账户" />
          <q-tab name="character" icon="accessibility" label="角色" />
          <q-tab name="item" icon="dehaze" label="物品" />
        </q-tabs>
      </template>

      <template v-slot:after>
        <q-tab-panels
          class="transparent text-white fit"
          v-model="tab"
          animated
          transition-prev="jump-up"
          transition-next="jump-up"
        >
          <q-tab-panel name="dashboard">
            <div class="row">
              <div class="col">
                <q-card class="transparent text-white">
                  <q-card-section>
                    <div class="row" style="height: 200px">
                      <div class="col-4 relative-position">
                        <q-circular-progress
                          show-value
                          class="text-white absolute-center"
                          value=100
                          size="180px"
                          color="light-blue"
                        />
                      </div>

                      <div class="col-4 relative-position">
                        <q-circular-progress
                          show-value
                          class="text-white absolute-center"
                          value=100
                          size="180px"
                          color="light-blue"
                        />
                      </div>

                      <div class="col-4 relative-position">
                        <q-circular-progress
                          show-value
                          class="text-white absolute-center"
                          value=100
                          size="180px"
                          color="light-blue"
                        />
                      </div>
                    </div>
                  </q-card-section>
                </q-card>
              </div>
            </div>
          </q-tab-panel>

          <q-tab-panel name="account">
            <div class="text-h4 q-mb-md">Alarms</div>
            <p>
              Lorem ipsum dolor sit, amet consectetur adipisicing elit. Quis
              praesentium cumque magnam odio iure quidem, quod illum numquam
              possimus obcaecati commodi minima assumenda consectetur culpa fuga
              nulla ullam. In, libero.
            </p>
            <p>
              Lorem ipsum dolor sit, amet consectetur adipisicing elit. Quis
              praesentium cumque magnam odio iure quidem, quod illum numquam
              possimus obcaecati commodi minima assumenda consectetur culpa fuga
              nulla ullam. In, libero.
            </p>
          </q-tab-panel>

          <q-tab-panel name="character">
            <div class="text-h4 q-mb-md">Movies</div>
            <p>
              Lorem ipsum dolor sit, amet consectetur adipisicing elit. Quis
              praesentium cumque magnam odio iure quidem, quod illum numquam
              possimus obcaecati commodi minima assumenda consectetur culpa fuga
              nulla ullam. In, libero.
            </p>
            <p>
              Lorem ipsum dolor sit, amet consectetur adipisicing elit. Quis
              praesentium cumque magnam odio iure quidem, quod illum numquam
              possimus obcaecati commodi minima assumenda consectetur culpa fuga
              nulla ullam. In, libero.
            </p>
            <p>
              Lorem ipsum dolor sit, amet consectetur adipisicing elit. Quis
              praesentium cumque magnam odio iure quidem, quod illum numquam
              possimus obcaecati commodi minima assumenda consectetur culpa fuga
              nulla ullam. In, libero.
            </p>
          </q-tab-panel>

          <q-tab-panel name="item">
            <div class="text-h4 q-mb-md">Movies</div>
            <p>
              Lorem ipsum dolor sit, amet consectetur adipisicing elit. Quis
              praesentium cumque magnam odio iure quidem, quod illum numquam
              possimus obcaecati commodi minima assumenda consectetur culpa fuga
              nulla ullam. In, libero.
            </p>
            <p>
              Lorem ipsum dolor sit, amet consectetur adipisicing elit. Quis
              praesentium cumque magnam odio iure quidem, quod illum numquam
              possimus obcaecati commodi minima assumenda consectetur culpa fuga
              nulla ullam. In, libero.
            </p>
            <p>
              Lorem ipsum dolor sit, amet consectetur adipisicing elit. Quis
              praesentium cumque magnam odio iure quidem, quod illum numquam
              possimus obcaecati commodi minima assumenda consectetur culpa fuga
              nulla ullam. In, libero.
            </p>
          </q-tab-panel>
        </q-tab-panels>
      </template>
    </q-splitter>
  </q-page>
</template>

<script>
export default {
  name: "PageAdmin",

  data() {
    return {
      splitterModel: 10,
      tab: "dashboard"
    };
  },

  created() {},

  methods: {},

  mounted() {},

  destroyed() {}
};
</script>

<style></style>
