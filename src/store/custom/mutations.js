export function login(state, value) {
  state.logined = value.logined;
  state.username = value.username;
  state.password = value.password;
}
