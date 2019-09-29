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
                    >
                      {{ dashboard.cpu }}%
                    </q-circular-progress>
                  </div>

                  <div class="col-4 text-center">
                    <div class="text-h4">内存</div>
                    <q-circular-progress
                      show-value
                      :value="dashboard.memory"
                      size="180px"
                      color="light-blue"
                    >
                      {{ dashboard.memory }}%
                    </q-circular-progress>
                  </div>

                  <div class="col-4 text-center">
                    <div class="text-h4">硬盘</div>
                    <q-circular-progress
                      show-value
                      :value="dashboard.disk"
                      size="180px"
                      color="light-blue"
                    >
                      {{ dashboard.disk }}%
                    </q-circular-progress>
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
                    >
                      {{ dashboard.backend }}%
                    </q-circular-progress>
                  </div>

                  <div class="col-3 text-center">
                    <div class="text-h4">数据库</div>
                    <q-circular-progress
                      show-value
                      :value="dashboard.database"
                      size="180px"
                      color="light-blue"
                    >
                      {{ dashboard.database }}%
                    </q-circular-progress>
                  </div>

                  <div class="col-3 text-center">
                    <div class="text-h4">登录服务端</div>
                    <q-circular-progress
                      show-value
                      :value="dashboard.loginserver"
                      size="180px"
                      color="light-blue"
                    >
                      {{ dashboard.loginserver }}%
                    </q-circular-progress>
                  </div>

                  <div class="col-3 text-center">
                    <div class="text-h4">游戏服务端</div>
                    <q-circular-progress
                      show-value
                      :value="dashboard.gameserver"
                      size="180px"
                      color="light-blue"
                    >
                      {{ dashboard.gameserver }}%
                    </q-circular-progress>
                  </div>
                </div>
              </div>

              <div class="col" style="margin: 30px" />

              <div class="col">
                <div class="row">
                  <div class="col">
                    <div class="text-h4 text-center">
                      操作系统：<br />{{ dashboard.os }}
                    </div>
                  </div>

                  <div class="col">
                    <div class="text-h4 text-center">
                      开机时间：<br />{{ dashboard.startTime }}
                    </div>
                  </div>

                  <div class="col">
                    <div class="text-h4 text-center">
                      运行时长：<br />{{ dashboard.runTime }}
                    </div>
                  </div>

                  <div class="col">
                    <div class="text-h4 text-center">
                      在线人数：<br />{{ dashboard.onlineNumber }}
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

import systemInfo from "systeminformation";
import os from "os";

export default {
  name: "PageAdmin",

  data() {
    return {
      splitterModel: 10,
      tab: "dashboard",
      dashboard: {
        os: "",
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

  methods: {
    // dateToString(time) {
    //   let datetime = new Date(time);
    //   let year = datetime.getFullYear();
    //   let month = datetime.getMonth() + 1;
    //   let date = datetime.getDate();
    //   let hour = datetime.getHours();
    //   let min = datetime.getMinutes();
    //   let sec = datetime.getSeconds();
    //   month = month < 10 ? "0" + month : month;
    //   date = date < 10 ? "0" + date : date;
    //   hour = hour < 10 ? "0" + hour : hour;
    //   min = min < 10 ? "0" + min : min;
    //   sec = sec < 10 ? "0" + sec : sec;
    //   return year + "-" + month + "-" + date + "\n" + hour + ":" + min + ":" + sec;
    // },

    elapsedTime(startTime, endTime) {
      let datetime1 = new Date(startTime);
      let hour1 = datetime1.getHours();
      let min1 = datetime1.getMinutes();
      let sec1 = datetime1.getSeconds();

      let datetime2 = new Date(endTime);
      let hour2 = datetime2.getHours();
      let min2 = datetime2.getMinutes();
      let sec2 = datetime2.getSeconds();

      let hour = hour2 - hour1;
      let min = min2 - min1;
      let sec = sec2 - sec1;
      hour = hour < 10 ? "0" + hour : hour;
      min = min < 10 ? "0" + min : min;
      sec = sec < 10 ? "0" + sec : sec;

      return hour + ":" + min + ":" + sec;
    }
  },

  created() {
    let datetime = new Date(os.uptime());
    let hour = datetime.getHours();
    let min = datetime.getMinutes();
    let sec = datetime.getSeconds();
    hour = hour < 10 ? "0" + hour : hour;
    min = min < 10 ? "0" + min : min;
    sec = sec < 10 ? "0" + sec : sec;
    let time = hour + ":" + min + ":" + sec;
    this.dashboard.startTime = time;

    setInterval(() => {
      systemInfo.osInfo(cb => {
        this.dashboard.os = cb.platform + " " + cb.build;
      });
      systemInfo.currentLoad(cb => {
        this.dashboard.cpu = parseInt(cb.currentload);
      });
      systemInfo.mem(cb => {
        this.dashboard.memory = parseInt((cb.used / cb.total) * 100);
      });
      systemInfo.fsSize(cb => {
        let size = 0,
          total = 0;
        for (let index = 0; index < cb.length; index++) {
          total += parseInt(cb[index].size);
          size += parseInt(cb[index].used);
        }
        this.dashboard.disk = parseInt((size / total) * 100);
      });

      this.dashboard.backend = 14;
      this.dashboard.database = 18;
      this.dashboard.loginserver = 21;
      this.dashboard.gameserver = 37;

      this.dashboard.runTime = this.elapsedTime(os.uptime(), systemInfo.time().current);

      this.dashboard.onlineNumber = 0;
    }, 1000);
  },

  mounted() {},

  destroyed() {}
};
</script>

<style></style>
