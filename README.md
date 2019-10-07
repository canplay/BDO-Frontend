# BDO-Frontend

![Image text](https://github.com/canplay/BDO-Frontend/blob/master/preview.png)

Dev:

1. command: npm i
2. run [BDO-Backend](https://github.com/canplay/BDO-Backend)
3. modify './src/components/config.js' ip to your [BDO-Backend](https://github.com/canplay/BDO-Backend) ip
4. modify './launcher/launcher.cpp' ip to your [BDO-Backend](https://github.com/canplay/BDO-Backend) ip and build it
5. copy './launcher/output/launcher.exe' to your BDO client dirctory and run once
6. command: quasar dev

Dist:

1. repeat steps 1-4 of Dev
2. command: quasar build
3. copy "./dist" to './web' in [BDO-Backend](https://github.com/canplay/BDO-Backend)
4. copy './launcher/output/launcher.exe' to './web/download' in [BDO-Backend](https://github.com/canplay/BDO-Backend)

Language:

- 简体中文: './src/i18n/zh-cn/index.js'
- English: './src/i18n/en-us/index.js'

the default language in MyLayout.vue:

```js
this.$i18n.locale = "zh-cn";
```

User:

1. download launcher.exe and run once
2. login BDO-Frontend
3. Launcher
