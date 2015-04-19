struct user {
  int id;
  struct user *next;
};

typedef struct user User;

User u, *lu;

User *go2(int id) {
  User *p = &u;

  while(p) {
    if (p->id == id) {
      goto end;
    }
    else
      p = p->next;
  }

  u.id = id;
end:
  lu = p;
  return p;
}