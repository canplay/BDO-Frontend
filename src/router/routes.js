const routes = [
  {
    path: "/",
    component: () => import("layouts/MyLayout.vue"),
    children: [
      { path: "", component: () => import("pages/Index.vue") },
      { path: "admin", component: () => import("pages/Admin.vue") },
      { path: "download", component: () => import("pages/Download.vue") },
      { path: "about", component: () => import("pages/About.vue") },
      { path: "shop", component: () => import("pages/Shop.vue") },
      { path: "view", component: () => import("pages/View.vue") }
    ]
  }
];

// Always leave this as last one
if (process.env.MODE !== "ssr") {
  routes.push({
    path: "*",
    component: () => import("pages/Error404.vue")
  });
}

export default routes;
