# reasonml-country-select

This project showcases how to write a React component that selects a country from a large list using [Melange](https://github.com/melange-re/melange) with [opam](https://opam.ocaml.org/).

## Quick Start

```shell
make init

# In separate terminals:
make watch
make serve
```

When running `make init`, you may encounter an error like this:

```
[ERROR] Could not determine which packages to install for this switch:
  * Missing dependency:
    - melange >= 1.0.0
    no matching version
```

To address this, first run `opam update`, then rerun `make init`.

## Commands

You can see all available commands by running `make help` or just `make`. Here are a few of the most useful ones:

- `make init`: set up opam local switch and download OCaml, Melange and JavaScript dependencies
- `make install`: install OCaml, Melange and JavaScript dependencies
- `make watch`: watch for the filesystem and have Melange rebuild on every change
- `make serve`: serve the application with a local HTTP server

## Opinionated ReasonML

In this repo there are some pretty opinionated options for implementing this design. Namely things like:

- Data last and pipe last `(|>)`
- Heavy use of `map`, `flatMap`, `fold` instead of switches
- All effectful code is wrapped in an `IO.t('a, 'e)`
- Wrapping `React.useX` hooks in things that help leverage the type system to prevent run-time errors and wrap effectful code in `IO.t('a, 'e)`
- Use of "point-free" notation via `(>>)` to create "data pipelines"

None of this is mandatory of course but used to illustrate a different way to write ReasonML code that I have found a lot of success with from both maintaining a codebase to scale linearly with respect to the number of features it has and with scaling a team of engineers into working on it.
