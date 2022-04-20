/* Any copyright is dedicated to the Public Domain.
   http://creativecommons.org/publicdomain/zero/1.0/ */

const { TodoList } = ChromeUtils.import("resource://gre/modules/components-utils/Todolist.jsm");

add_task(async function() {
    const todo = await TodoList.init();
    await todo.addItem("Write JS bindings");
    todo.destroy();
    try {
        await todo.addItem("Should throw! Todo was destroyed");
        Assert.fail()
    } catch (e) {
        // OK todo was destroyed
    }
});
