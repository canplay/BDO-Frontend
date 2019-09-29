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
          class="transparent text-teal fit"
          v-model="tab"
          animated
          transition-prev="jump-up"
          transition-next="jump-up"
        >
          <q-tab-panel name="dashboard">
            <div class="column">
              <div class="col">
                <div class="row">
                  <div class="col-4 text-center">
                    <div class="text-h4">CPU</div>
                    <q-circular-progress
                      show-value
                      :value="dashboard.cpu"
                      size="180px"
                      color="light-blue"
                    />
                  </div>

                  <div class="col-4 text-center">
                    <div class="text-h4">内存</div>
                    <q-circular-progress
                      show-value
                      :value="dashboard.memory"
                      size="180px"
                      color="light-blue"
                    />
                  </div>

                  <div class="col-4 text-center">
                    <div class="text-h4">硬盘</div>
                    <q-circular-progress
                      show-value
                      :value="dashboard.disk"
                      size="180px"
                      color="light-blue"
                    />
                  </div>
                </div>
              </div>

              <div class="col-1" style="margin: 20px" />

              <div class="col">
                <div class="row">
                  <div class="col-3 text-center">
                    <div class="text-h4">后端</div>
                    <q-circular-progress
                      show-value
                      :value="dashboard.backend"
                      size="180px"
                      color="light-blue"
                    />
                  </div>

                  <div class="col-3 text-center">
                    <div class="text-h4">数据库</div>
                    <q-circular-progress
                      show-value
                      :value="dashboard.database"
                      size="180px"
                      color="light-blue"
                    />
                  </div>

                  <div class="col-3 text-center">
                    <div class="text-h4">登录服务端</div>
                    <q-circular-progress
                      show-value
                      :value="dashboard.loginserver"
                      size="180px"
                      color="light-blue"
                    />
                  </div>

                  <div class="col-3 text-center">
                    <div class="text-h4">游戏服务端</div>
                    <q-circular-progress
                      show-value
                      :value="dashboard.gameserver"
                      size="180px"
                      color="light-blue"
                    />
                  </div>
                </div>
              </div>

              <div class="col" style="margin: 30px" />

              <div class="col">
                <div class="row">
                  <div class="col">
                    <div class="text-h4 text-center">
                      开机时间：{{ dashboard.startTime }}
                    </div>
                  </div>

                  <div class="col">
                    <div class="text-h4 text-center">
                      运行时长：{{ dashboard.runTime }}
                    </div>
                  </div>

                  <div class="col">
                    <div class="text-h4 text-center">
                      在线人数：{{ dashboard.onlineNumber }}
                    </div>
                  </div>
                </div>
              </div>
            </div>
          </q-tab-panel>

          <q-tab-panel name="account">
            <q-table
              class="fit"
              :data="account.data"
              :columns="account.columns"
              :rows-per-page-options="[]"
              row-key="id"
            >
              <template v-slot:body="props">
                <q-tr :props="props">
                  <q-td key="id" :props="props">
                    {{ props.row.id }}
                  </q-td>

                  <q-td key="username" :props="props">
                    {{ props.row.username }}
                    <q-popup-edit v-model="props.row.username">
                      <q-input v-model="props.row.username" dense autofocus />
                    </q-popup-edit>
                  </q-td>

                  <q-td key="email" :props="props">
                    {{ props.row.email }}
                    <q-popup-edit v-model="props.row.email">
                      <q-input v-model="props.row.email" dense autofocus />
                    </q-popup-edit>
                  </q-td>

                  <q-td key="password" :props="props">
                    {{ props.row.password }}
                  </q-td>

                  <q-td key="pin" :props="props">
                    {{ props.row.pin }}
                    <q-popup-edit v-model="props.row.pin">
                      <q-input v-model="props.row.pin" dense autofocus />
                    </q-popup-edit>
                  </q-td>

                  <q-td key="registerDate" :props="props">
                    {{ props.row.registerDate }}
                    <q-popup-edit v-model="props.row.registerDate">
                      <q-input
                        v-model="props.row.registerDate"
                        dense
                        autofocus
                      />
                    </q-popup-edit>
                  </q-td>

                  <q-td key="ip" :props="props">
                    {{ props.row.ip }}
                    <q-popup-edit v-model="props.row.ip">
                      <q-input v-model="props.row.ip" dense autofocus />
                    </q-popup-edit>
                  </q-td>

                  <q-td key="level" :props="props">
                    {{ props.row.level }}
                    <q-popup-edit v-model="props.row.level">
                      <q-input v-model="props.row.level" dense autofocus />
                    </q-popup-edit>
                  </q-td>
                </q-tr>
              </template>
            </q-table>
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
const columns = [
  { name: "id", align: "left", label: "ID", field: "id" },
  { name: "username", align: "left", label: "用户名", field: "username" },
  { name: "email", align: "left", label: "邮件", field: "email" },
  { name: "password", align: "left", label: "密码", field: "password" },
  { name: "pin", align: "left", label: "Pin", field: "pin" },
  {
    name: "registerDate",
    align: "left",
    label: "注册时间",
    field: "registerDate"
  },
  { name: "ip", align: "left", label: "IP地址", field: "ip" },
  { name: "level", align: "left", label: "账号等级", field: "level" }
];

const data = [
  {
    id: 0,
    username: "Frozen Yogurt",
    email: "123@qq.com",
    password: 159,
    pin: 0,
    registerDate: 24,
    ip: "127.0.0.1",
    level: 4
  }
];

export default {
  name: "PageAdmin",

  data() {
    return {
      splitterModel: 10,
      tab: "dashboard",
      dashboard: {
        cpu: 0,
        memory: 0,
        disk: 0,
        backend: 0,
        database: 0,
        loginserver: 0,
        gameserver: 0,
        startTime: "00:00:00",
        runTime: "00:00:00",
        onlineNumber: 0
      },
      account: {
        data,
        columns
      }
    };
  },

  created() {
    this.dashboard.cpu = 31;
    this.dashboard.memory = 24;
    this.dashboard.disk = 38;
    this.dashboard.backend = 14;
    this.dashboard.database = 18;
    this.dashboard.loginserver = 21;
    this.dashboard.gameserver = 37;
    this.dashboard.startTime = "15:03:33";
    this.dashboard.runTime = "00:01:21";
    this.dashboard.onlineNumber = 0;
  },

  methods: {},

  mounted() {},

  destroyed() {}
};
</script>

<style></style>
