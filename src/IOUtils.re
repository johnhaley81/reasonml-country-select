// So while at Qwick, we noticed that having events be things like 'a => unit was a large surface area for bugs and
// to solve this we started wrapping any dom events in an IO that would be executed by the lowest level dom element
// that accepted the IO. In doing so this solved many race conditions we were faced with throughout the code base
// and allowed for greater flexibility in composing event handlers. This is obviously not a universal thing but I
// wanted to illustrate it here in this project to showcase a problem we ran into and a solution we came up with
// to solve it. Happy to discuss it more if you are curious.
let unsafeRunHandledAsync =
  IO.unsafeRunAsync(Result.fold(() => (), () => ()));
