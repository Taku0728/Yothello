// workbox
importScripts(
  "https://storage.googleapis.com/workbox-cdn/releases/4.3.1/workbox-sw.js"
);
if (workbox) {
  workbox.routing.registerRoute(
    "https://othello.to-kei.net/weak/",
    new workbox.strategies.NetworkFirst()
  );
}
