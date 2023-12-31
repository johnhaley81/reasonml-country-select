let extendBaseStyle = (baseStyle, extendedStyle) =>
  [Some(baseStyle), extendedStyle] |> List.catOptions |> Css.merge;

// At Qwick we tried to follow the "least powerful tool for the job" philosophy and how we interpretted that was to
// default to patterns that gave you the least amount of data you needed in a specific part of the code and to leverage
// more IO.t('a, 'e) instead of things like unit. What we learned was this provided a lot more context into the
// thoughts of the developer when they were writing the code. It also helped code that was more complex really stick
// and let the developer know that this area required a bit more attention than others.
//
// In following that philosophy, we wrapped all of the internal react hooks to both provide less data unless needed,
// to leverage IO to reduce race conditions, and to leverage the type system in ways that also reduced run-time errors

let useMappableState = intitalState => {
  let (state, set) = React.useState(intitalState);
  let set =
    React.useCallback0(newStateFn =>
      IO.suspend(() => set(newStateFn)) |> IO.mapError(ignore)
    );

  (state, set);
};

let useState = getInitialValue => {
  let (value, map) = useMappableState(getInitialValue);

  let set = React.useCallback0(const >> map);

  (value, set);
};

let useMemo0 = React.useMemo0;
let useMemo1 = (fn, memoizedProps) =>
  memoizedProps |> Array.pure |> React.useMemo1(() => fn(memoizedProps));

let%private useIOWrapper = (~onUnmount=?, ~onFinish=?, io) => {
  let ioResult = ref(None);

  io
  |> IO.unsafeRunAsync(result => {
       ioResult := result |> Option.pure;
       result |> Option.pure |> Option.apply(onFinish) |> Option.getOrElse();
     });

  Some(
    () =>
      ioResult^
      |> Option.apply(onUnmount)
      |> Option.getOrElseLazy(IO.pure)
      |> IOUtils.unsafeRunHandledAsync,
  );
};
let useEffect0 = (~onUnmount=?, ~onFinish=?, io) =>
  React.useEffect0(() => io |> useIOWrapper(~onUnmount?, ~onFinish?));
let useEffect1 = (~onUnmount=?, ~onFinish=?, io, arg) =>
  arg
  |> Array.pure
  |> React.useEffect1(() =>
       arg |> io |> useIOWrapper(~onUnmount?, ~onFinish?)
     );
let useEffect2 = (~onUnmount=?, ~onFinish=?, io, args) =>
  args
  |> React.useEffect2(() =>
       args
       |> Relude.Function.uncurry2(io)
       |> useIOWrapper(~onUnmount?, ~onFinish?)
     );

// While at Qwick, we found that working with refs were a little wonky, requiring a weird interface with
// initializing it with React.useRef(Js.Nullable.null) but then having to pass it into the dom element
// with <div ref={ReactDOM.Ref.domRef(divRef)} />. We also rarely used Js.Nullable so we did a lot of conversion
// with Js.Nullable.t('a) to an option('a) so this takes care of that. Also since it was something that is
// mutated and accessing the DOM directly (usually), we were able to solve some weird edge-cases by wrapping
// it inside of an IO
let useRef:
  'a 'b.
  (React.ref(Js.Nullable.t('a)) => 'b) => (IO.t(option('a), unit), 'b)
 =
  setter => {
    let genericRef = React.useRef(Js.Nullable.null);

    let getCurrentRef: 'a. React.ref(Js.Nullable.t('a)) => option('a) =
      ({current}) => current |> Js.Nullable.toOption;

    let refInIO = useMemo1(IO.pure >> IO.map(getCurrentRef), genericRef);
    (refInIO, genericRef |> setter);
  };

let useDomRef = () => useRef(ReactDOM.Ref.domRef);

// We wrapped all of the React.string, React.null, etc... in components to provide a more consistent interface
// in the codebase. This also allowed us to do things in the typesystem to avoid more runtime errors (like providing keys
// for elements in an array)
module S = {
  [@react.component]
  let make = (~children) => React.string(children);
};

module F = {
  [@react.component]
  let make = (~children, ~formatter=Float.toString) =>
    <S> {children |> formatter} </S>;
};

module Null = {
  [@react.component]
  let make = () => React.null;
};

module Spread = {
  [@react.component]
  let make = (~props, ~children) => React.cloneElement(children, props);
};
