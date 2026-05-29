module frontend;

import std::collections::object;
import std::encoding::json;
import std::io;

import bgfx;
import webview;

macro @generate(#components) {
    var $y = "";
    var $x = #components;

    $foreach $item: $x:
        $y = $y +++ $item +++ "\n";
    $endforeach

   return $y;
}

macro @components() {
   return @generate({
        $embed("components/notification.js"),
   });
}

macro @pages() {
   return @generate({
        $embed("pages/index.js"),
   });
}

macro @index() {
    return `<!DOCTYPE html>
            <html lang="en" data-theme="dark">
              <head>
                <meta charset="utf-8">
                <meta name="viewport" content="width=device-width, initial-scale=1">
                <script type="importmap">
                  {
                    "imports": {
                      "preact": "https://esm.sh/preact@10.23.1",
                      "preact/": "https://esm.sh/preact@10.23.1/",
                      "@preact/signals": "https://esm.sh/@preact/signals@1.3.0?external=preact",
                      "htm/preact": "https://esm.sh/htm@3.1.1/preact?external=preact"
                    }
                  }
                </script>
                <link href="https://cdn.jsdelivr.net/npm/daisyui@5" rel="stylesheet" type="text/css" />
                <script src="https://cdn.jsdelivr.net/npm/@tailwindcss/browser@4"></script>
                <style type="text/tailwindcss">`+++$embed("app.css")+++`</style>
                <title>VGEngine AI</title>
              </head>
              <body>
                <div id="app"></div>
                <script type="module">
                    import { render } from "preact";
                    import { useEffect } from "preact/hooks";
                    import { html } from "htm/preact";

                    // Load components
                    `+++@components()+++`

                    // Load each page
                    `+++@pages()+++`

                    function App() {
                      useEffect(() => {
                        const loop = (delta) => {
                          window.render(delta).then(() => {
                            requestAnimationFrame(loop);
                          });
                        };

                        addEventListener("resize", () =>
                          window.on_resize(window.innerWidth, window.innerHeight),
                        );

                        requestAnimationFrame(loop);
                      }, []);

                        return html`+++"`<div class='w-screen h-screen'><${IndexPage}/></div>`"+++`
                    }

                    render(html`+++"`<${App}/>`"+++`, document.getElementById('app'));
                </script>
              </body>
            </html>
`;
}

struct FrontendContext {
    webview::Webview webview;
    int width;
    int height;
    int refreshRate;
}

fn void on_resize(ZString id, ZString req, void* arg) @private {
    FrontendContext* ctx = (FrontendContext*) arg;
    object::Object* result = json::tparse(req.str_view())!!;

    if(result == null) {
        // TODO: Log this, it should't happend
        webview::return_bind(ctx.webview, id, -1, "");
        return;
    }

    defer result.free();

    if(!result.is_array()) {
        // TODO: Log this, it should't happend
        webview::return_bind(ctx.webview, id, -1, "");
        return;
    }

    if(result.get_len() != 2) {
        // TODO: Log this, it should't happend
        webview::return_bind(ctx.webview, id, -1, "");
        return;
    }

    ctx.width = result.get_int_at(0)!!;
    ctx.height = result.get_int_at(1)!!;

    // TODO: Remove this from here, it should be part of the renderer and not the frontend
    bgfx::reset((uint) ctx.width, (uint) ctx.height, (uint) bgfx::ResetFlags.VSYNC, bgfx::TextureFormat.BC1);
    webview::return_bind(ctx.webview, id, 0, "");
}

macro int run(
    int width,
    int height,
    int refresh_rate,
    void* init_renderer,
    void* renderer
) {
    FrontendContext ctx = {};
    ctx.webview = webview::create(1,  null);
    ctx.width = width;
    ctx.height = height;
    ctx.refreshRate = refresh_rate;

    webview::set_size(ctx.webview, ctx.width, ctx.height, webview::WebviewHint.NONE);
    webview::bind(ctx.webview, "on_resize", &on_resize, &ctx);
    webview::bind(ctx.webview, "render", renderer, &ctx);
    webview::dispatch(ctx.webview, init_renderer, &ctx);
    webview::set_html(ctx.webview, @index());

    // Blocks while is running
    webview::WebviewError wv_error = webview::run(ctx.webview);

    if (wv_error != WebviewError.OK) {
        int code = (int) wv_error;
        io::printf("failed to exit gracefully, webview error code: %d", code);
        return code;
    }

    webview::destroy(ctx.webview);
    return 0;
}
