let blurRef = domRef => domRef->ReactDOM.domElementToObj##blur();
let blurRef = domRef => domRef |> IO.map(Option.fold((), blurRef));
let focusRef = domRef => domRef->ReactDOM.domElementToObj##focus();
let focusRef = domRef => domRef |> IO.map(Option.fold((), focusRef));

[@bs.deriving accessors]
type key =
  | EscapeKey
  | EnterKey
  | ArrowUp
  | ArrowDown;

let fromString =
  fun
  | "Enter" => enterKey |> Option.pure
  | "Escape" => escapeKey |> Option.pure
  | "ArrowUp" => arrowUp |> Option.pure
  | "ArrowDown" => arrowDown |> Option.pure
  | _ => None;
let getKeyFromEvent = e =>
  e |> ReactEvent.Keyboard.key |> fromString |> Option.map(key => (key, e));

module IntersectionObserver = {
  module IntersectionObserverEntry = {
    type t;

    [@bs.get] external isIntersecting: t => bool = "isIntersecting";
  };

  type t;

  [@bs.new]
  external make: (array(IntersectionObserverEntry.t) => unit) => t =
    "IntersectionObserver";
  let make = callback => make(callback >> IOUtils.unsafeRunHandledAsync);

  [@bs.send] external observe: (t, Dom.element) => unit = "observe";
  let observe = (el, t) =>
    IO.suspend(() => observe(t, el)) |> IO.map(() => t);

  [@bs.send] external disconnect: t => unit = "disconnect";
  let disconnect = t => IO.suspend(() => t |> disconnect);
};

module ScrollOptions = {
  [@bs.deriving jsConverter]
  type behavior = [ | `auto | `smooth | `instant];

  [@bs.deriving jsConverter]
  type block = [ | `start | `center | [@bs.as "end"] `end_ | `nearest];

  [@bs.deriving jsConverter]
  type inline = [ | `start | `center | [@bs.as "end"] `end_ | `nearest];

  [@bs.deriving jsConverter]
  type scrollMode = [ | `always | [@bs.as "if-needed"] `ifNeeded];

  [@bs.deriving abstract]
  type t = {
    [@bs.optional]
    behavior: string,
    [@bs.optional]
    block: string,
    [@bs.optional]
    inline: string,
    [@bs.optional]
    scrollMode: string,
  };

  let make = (~behavior=?, ~block=?, ~inline=?, ~scrollMode=?) =>
    t(
      ~behavior=?behavior |> Option.map(behaviorToJs),
      ~block=?block |> Option.map(blockToJs),
      ~inline=?inline |> Option.map(inlineToJs),
      ~scrollMode=?scrollMode |> Option.map(scrollModeToJs),
    );

  let behaviorGet = behaviorGet >> Option.flatMap(behaviorFromJs);
  let blockGet = blockGet >> Option.flatMap(blockFromJs);
  let inlineGet = inlineGet >> Option.flatMap(inlineFromJs);
  let scrollModeGet = scrollModeGet >> Option.flatMap(scrollModeFromJs);
};

[@bs.module "smooth-scroll-into-view-if-needed"]
external scrollRefIntoView: (Dom.element, ScrollOptions.t) => unit = "default";
let scrollRefIntoView =
  Option.getOrElse(ScrollOptions.make()) >> flip(scrollRefIntoView);
let scrollRefIntoView = (~scrollOptions=?) =>
  IO.map(Option.fold((), scrollRefIntoView(scrollOptions)));

let scrollRefIntoViewOnCondition = (~scrollOptions=?, condition, domRef) =>
  condition ? domRef |> scrollRefIntoView(~scrollOptions?) : IO.pure();
