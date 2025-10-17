import tkinter as tk
from tkinter import ttk, messagebox

# 🎨 --- Global Style Settings ---
BG_COLOR = "#f4f6f8"
CARD_COLOR = "#ffffff"
HEADER_COLOR = "#0078D7"
TEXT_COLOR = "#333333"
DONE_COLOR = "#d3ffd3"
DARK_BG = "#1E1E1E"
DARK_CARD = "#2D2D2D"
DARK_TEXT = "#FFFFFF"


class ToDoApp(tk.Tk):
    def __init__(self):
        super().__init__()
        self.title("✨ To-Do List App")
        self.state('normal')  # Open maximized
        self.resizable(True, True)

        # Theme
        self.theme = "light"

        # Full-screen toggle
        self.fullscreen = False
        self.bind("<F11>", self.toggle_fullscreen)
        self.bind("<Escape>", self.end_fullscreen)

        # Container for all frames
        container = tk.Frame(self, bg=BG_COLOR)
        container.pack(fill="both", expand=True)

        self.frames = {}
        for F in (HomePage, ToDoPage, StatsPage, SettingsPage, AboutPage):
            frame = F(container, self)
            self.frames[F] = frame
            frame.grid(row=0, column=0, sticky="nsew")

        self.show_frame(HomePage)

    def show_frame(self, page):
        self.frames[page].tkraise()

    def toggle_theme(self):
        global BG_COLOR, CARD_COLOR, TEXT_COLOR
        if self.theme == "light":
            self.theme = "dark"
            BG_COLOR, CARD_COLOR, TEXT_COLOR = DARK_BG, DARK_CARD, DARK_TEXT
        else:
            self.theme = "light"
            BG_COLOR, CARD_COLOR, TEXT_COLOR = "#f4f6f8", "#ffffff", "#333333"
        # reload pages
        for frame in self.frames.values():
            frame.config(bg=BG_COLOR)
        self.show_frame(HomePage)

    def toggle_fullscreen(self, event=None):
        self.fullscreen = not self.fullscreen
        self.attributes("-fullscreen", self.fullscreen)

    def end_fullscreen(self, event=None):
        self.fullscreen = False
        self.attributes("-fullscreen", False)


# 🏠 --- HOME PAGE ---
class HomePage(tk.Frame):
    def __init__(self, parent, controller):
        super().__init__(parent, bg=BG_COLOR)
        tk.Label(self, text="📝 To-Do List App", font=("Segoe UI", 22, "bold"),
                 bg=BG_COLOR, fg=HEADER_COLOR).pack(pady=80)

        ttk.Button(self, text="Start To-Do List",
                   command=lambda: controller.show_frame(ToDoPage)).pack(pady=10, ipadx=10, ipady=5)
        ttk.Button(self, text="Statistics",
                   command=lambda: controller.show_frame(StatsPage)).pack(pady=10, ipadx=10, ipady=5)
        ttk.Button(self, text="Settings",
                   command=lambda: controller.show_frame(SettingsPage)).pack(pady=10, ipadx=10, ipady=5)
        ttk.Button(self, text="About App",
                   command=lambda: controller.show_frame(AboutPage)).pack(pady=10, ipadx=10, ipady=5)
        ttk.Button(self, text="Exit", command=controller.destroy).pack(pady=20, ipadx=10, ipady=5)


# ✅ --- TO-DO PAGE ---
class ToDoPage(tk.Frame):
    def __init__(self, parent, controller):
        super().__init__(parent, bg=BG_COLOR)

        header = tk.Frame(self, bg=HEADER_COLOR, height=60)
        header.pack(fill="x")
        tk.Label(header, text="Your Tasks", bg=HEADER_COLOR,
                 fg="white", font=("Segoe UI", 18, "bold")).pack(pady=10)

        add_frame = tk.Frame(self, bg=BG_COLOR)
        add_frame.pack(pady=15)
        self.task_entry = ttk.Entry(add_frame, width=35, font=("Segoe UI", 12))
        self.task_entry.grid(row=0, column=0, padx=5)
        ttk.Button(add_frame, text="Add Task", command=self.add_task).grid(row=0, column=1, padx=5)

        # Scroll area
        self.canvas = tk.Canvas(self, bg=BG_COLOR, highlightthickness=0)
        self.scroll_frame = tk.Frame(self.canvas, bg=BG_COLOR)
        self.scrollbar = ttk.Scrollbar(self, orient="vertical", command=self.canvas.yview)
        self.canvas.configure(yscrollcommand=self.scrollbar.set)

        self.canvas.create_window((0, 0), window=self.scroll_frame, anchor="nw")
        self.scroll_frame.bind("<Configure>", lambda e: self.canvas.configure(scrollregion=self.canvas.bbox("all")))

        self.canvas.pack(side="left", fill="both", expand=True, padx=10, pady=5)
        self.scrollbar.pack(side="right", fill="y")

        # Bottom Nav
        nav = tk.Frame(self, bg=BG_COLOR)
        nav.pack(pady=10)
        ttk.Button(nav, text="🏠 Home", command=lambda: controller.show_frame(HomePage)).grid(row=0, column=0, padx=5)
        ttk.Button(nav, text="📊 Stats", command=lambda: controller.show_frame(StatsPage)).grid(row=0, column=1, padx=5)

        self.tasks = []

    def add_task(self):
        text = self.task_entry.get().strip()
        if not text:
            messagebox.showwarning("Warning", "Please enter a task!")
            return

        task_card = tk.Frame(self.scroll_frame, bg=CARD_COLOR, bd=1, relief="solid")
        task_card.pack(fill="x", padx=8, pady=6)

        var = tk.BooleanVar()
        check = tk.Checkbutton(task_card, text=text, variable=var, bg=CARD_COLOR,
                               font=("Segoe UI", 12), anchor="w", command=lambda: self.toggle_done(check, var))
        check.pack(side="left", fill="x", expand=True, padx=10, pady=8)

        del_btn = ttk.Button(task_card, text="🗑️", width=3, command=lambda: self.delete_task(task_card))
        del_btn.pack(side="right", padx=8)

        self.tasks.append((task_card, var))
        self.task_entry.delete(0, tk.END)

    def toggle_done(self, check, var):
        if var.get():
            check.config(fg="gray", font=("Segoe UI", 12, "overstrike"))
        else:
            check.config(fg=TEXT_COLOR, font=("Segoe UI", 12))

    def delete_task(self, frame):
        frame.destroy()
        self.tasks = [t for t in self.tasks if t[0] != frame]


# 📊 --- STATISTICS PAGE ---
class StatsPage(tk.Frame):
    def __init__(self, parent, controller):
        super().__init__(parent, bg=BG_COLOR)

        header = tk.Frame(self, bg=HEADER_COLOR, height=60)
        header.pack(fill="x")
        tk.Label(header, text="Task Statistics", bg=HEADER_COLOR,
                 fg="white", font=("Segoe UI", 18, "bold")).pack(pady=10)

        self.info_label = tk.Label(self, text="", bg=BG_COLOR, fg=TEXT_COLOR, font=("Segoe UI", 13))
        self.info_label.pack(pady=40)

        ttk.Button(self, text="Refresh Stats", command=lambda: self.update_stats(controller)).pack(pady=10)
        ttk.Button(self, text="Back to Home", command=lambda: controller.show_frame(HomePage)).pack(pady=10)

    def update_stats(self, controller):
        todo_page = controller.frames[ToDoPage]
        total = len(todo_page.tasks)
        done = sum(var.get() for _, var in todo_page.tasks)
        pending = total - done
        self.info_label.config(text=f"✅ Completed: {done}\n⌛ Pending: {pending}\n📋 Total Tasks: {total}")


# ⚙️ --- SETTINGS PAGE ---
class SettingsPage(tk.Frame):
    def __init__(self, parent, controller):
        super().__init__(parent, bg=BG_COLOR)
        header = tk.Frame(self, bg=HEADER_COLOR, height=60)
        header.pack(fill="x")
        tk.Label(header, text="Settings", bg=HEADER_COLOR,
                 fg="white", font=("Segoe UI", 18, "bold")).pack(pady=10)

        ttk.Button(self, text="🌗 Toggle Theme", command=controller.toggle_theme).pack(pady=20)
        ttk.Button(self, text="Back to Home", command=lambda: controller.show_frame(HomePage)).pack(pady=20)


# ℹ️ --- ABOUT PAGE ---
class AboutPage(tk.Frame):
    def __init__(self, parent, controller):
        super().__init__(parent, bg=BG_COLOR)
        header = tk.Frame(self, bg=HEADER_COLOR, height=60)
        header.pack(fill="x")
        tk.Label(header, text="About", bg=HEADER_COLOR,
                 fg="white", font=("Segoe UI", 18, "bold")).pack(pady=10)

        about_text = (
            "📘 About This App\n\n"
            "This To-Do List app helps you manage your daily tasks easily.\n"
            "You can:\n"
            "✅ Add and mark tasks done\n"
            "📊 View stats of completed tasks\n"
            "⚙️ Customize theme in Settings\n\n"
            "Developed for the HCI Project using Tkinter."
        )

        tk.Label(self, text=about_text, bg=BG_COLOR, fg=TEXT_COLOR,
                 justify="left", font=("Segoe UI", 12), wraplength=400).pack(padx=30, pady=40)

        ttk.Button(self, text="Back to Home", command=lambda: controller.show_frame(HomePage)).pack(pady=20)


# 🚀 --- Run App ---
if __name__ == "__main__":
    app = ToDoApp()
    app.mainloop()
