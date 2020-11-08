export function login(state, value, username, password) {
  state.logined = value;
  state.username = username;
  state.password = password;
}
