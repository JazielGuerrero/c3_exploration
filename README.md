# C3 Exploration

This repo contains multiple explorations/things I've done in **C3** while I try to build a game engine (which exists in another repo).

At the current state of this repo, you will find how to run the [webview](https://github.com/webview/webview)
on top of [bgfx](https://github.com/bkaradzic/bgfx). I have created the **C3 bindings** for bgfx and webview, which you
can find in the `src/bindings` directory. The repo also contains the **static library for bgfx and webview for Windows**.

> NOTE: I'm new to C3, and working at this low-level (C/C++). I'm trying to learn as much as I can; thus, feedback is more than welcome. Thanks!

---

## How it works

- I'm using **GLFW** to get the width and height of the window. I'm doing it this way because initially I wanted to use GLFW
  and mount the webview on top of it. However, I encounter some issues when passing the `nwh` to: `fn Webview create(int debug, void* window) @extern("webview_create");`

- With the window information, I then proceed to create the webview. I bind **two functions** to the webview:

  1. **`on_resize`** – tries to keep bgfx and webview in sync.  
     This function is called when the window is resized by listening to the resize event of the window.
Not working as expected yet.
  2. **`render`** – in charge of rendering bgfx. In `main.c3` you can see a function named `render` which is called from the webview, 
specially by [requestAnimationFrame](https://developer.mozilla.org/en-US/docs/Web/API/Window/requestAnimationFrame).

### UI architecture

I'm also trying to figure out a project structure/architecture where I can implement my UI components like if I
was using React (through Vite, Webpack, etc.), basically looking to have the same DX we have in Frontend.

- I created the `frontend` directory which contains 2 directories for the moment:
  - `components/`
  - `pages/`

- In `frontend/index.c3` I defined 2 macros:
  - `@pages`
  - `@components`

These macros are used to **import** the pages and components from the `frontend/pages` and `frontend/components`
directories and then **inject them as `<script></script>` tags** in the `index.html` file (which is embededded in the
executable in the `frontend/index.c3` file).


> I'm using [Preact](https://preactjs.com) for the UI components and for state management **Preact Signals** 
(which I don't know yet if it is going to work as I expect.). Also using TailwindCSS for styling.
