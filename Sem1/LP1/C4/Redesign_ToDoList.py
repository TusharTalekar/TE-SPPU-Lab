import tkinter as tk
from tkinter import ttk, messagebox
from tkinter import simpledialog
import matplotlib.pyplot as plt
from matplotlib.backends.backend_tkagg import FigureCanvasTkAgg

# 🎨 Global Styles
BG_COLOR = "#f4f6f8"
CARD_COLOR = "#ffffff"
HEADER_COLOR = "#0078D7"
TEXT_COLOR = "#333333"
DONE_COLOR = "#d3ffd3"
DARK_BG = "#1E1E1E"
DARK_CARD = "#2D2D2D"
DARK_TEXT = "#FFFFFF"

# Tooltip helper
class ToolTip:
    def __init__(self, widget, text):
        self.widget = widget
        self.text = text
        self.tip_window = None
        widget.bind("<Enter>", self.show_tip)
        widget.bind("<Leave>", self.hide_tip)

    def show_tip(self, event=None):
        if self.tip_window or not self.text:
            return
        x, y, _, cy = self.widget.bbox("insert")
        x = x + self.widget.winfo_rootx() + 25
        y = y + cy + self.widget.winfo_rooty() + 20
        self.tip_window = tw = tk.Toplevel(self.widget)
        tw.wm_overrideredirect(True)
        tw.wm_geometry(f"+{x}+{y}")
        label = tk.Label(tw, text=self.text, background="#ffffe0",
                         relief="solid", borderwidth=1,
                         font=("Segoe UI", 10))
        label.pack()

    def hide_tip(self, event=None):
        if self.tip_window:
            self.tip_window.destroy()
            self.tip_window = None


# --- Main App ---
class ToDoApp(tk.Tk):
    def __init__(self):
        super().__init__()
        self.title("✨ To-Do List App")
        self.state('normal')
        self.resizable(True, True)

        self.theme = "light"
        self.fullscreen = False
        self.deleted_tasks = []  # store deleted tasks for undo

        self.bind("<F11>", self.toggle_fullscreen)
        self.bind("<Escape>", self.end_fullscreen)
        self.bind("<Return>", self.add_task_shortcut)
        self.bind("h", lambda e: self.show_frame(HomePage))
        self.bind("H", lambda e: self.show_frame(HomePage))
        self.bind("t", lambda e: self.toggle_theme())
        self.bind("T", lambda e: self.toggle_theme())
        self.bind("r", lambda e: self.refresh_stats())
        self.bind("R", lambda e: self.refresh_stats())

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
        for frame in self.frames.values():
            frame.config(bg=BG_COLOR)
            frame.update_theme()
        self.show_frame(HomePage)

    def toggle_fullscreen(self, event=None):
        self.fullscreen = not self.fullscreen
        self.attributes("-fullscreen", self.fullscreen)

    def end_fullscreen(self, event=None):
        self.fullscreen = False
        self.attributes("-fullscreen", False)

    def add_task_shortcut(self, event=None):
        self.frames[ToDoPage].add_task()

    def refresh_stats(self):
        self.frames[StatsPage].update_stats(self)


# --- HOME PAGE ---
class HomePage(tk.Frame):
    def __init__(self, parent, controller):
        super().__init__(parent, bg=BG_COLOR)
        self.controller = controller

        tk.Label(self, text="📝 To-Do List App", font=("Segoe UI", 22, "bold"),
                 bg=BG_COLOR, fg=HEADER_COLOR).pack(pady=80)

        btns = [
            ("Start To-Do List", ToDoPage),
            ("Statistics", StatsPage),
            ("Settings", SettingsPage),
            ("About App", AboutPage),
            ("Exit", self.exit_app)
        ]

        for text, page in btns:
            b = ttk.Button(self, text=text, command=lambda p=page: self.show_page(p))
            b.pack(pady=10, ipadx=10, ipady=5)
            ToolTip(b, f"Click to {text.lower()}")

    def show_page(self, page):
        if page == self.exit_app:
            self.exit_app()
        else:
            self.controller.show_frame(page)

    def exit_app(self):
        if messagebox.askyesno("Exit", "Are you sure you want to exit?"):
            self.controller.destroy()


# --- TO-DO PAGE ---
class ToDoPage(tk.Frame):
    def __init__(self, parent, controller):
        super().__init__(parent, bg=BG_COLOR)
        self.controller = controller
        self.tasks = []

        header = tk.Frame(self, bg=HEADER_COLOR, height=60)
        header.pack(fill="x")
        tk.Label(header, text="Your Tasks", bg=HEADER_COLOR,
                 fg="white", font=("Segoe UI", 18, "bold")).pack(pady=10)

        add_frame = tk.Frame(self, bg=BG_COLOR)
        add_frame.pack(pady=15)
        self.task_entry = ttk.Entry(add_frame, width=35, font=("Segoe UI", 12))
        self.task_entry.grid(row=0, column=0, padx=5)
        add_btn = ttk.Button(add_frame, text="Add Task", command=self.add_task)
        add_btn.grid(row=0, column=1, padx=5)
        ToolTip(add_btn, "Click to add task")

        # Scrollable canvas
        self.canvas = tk.Canvas(self, bg=BG_COLOR, highlightthickness=0)
        self.scroll_frame = tk.Frame(self.canvas, bg=BG_COLOR)
        self.scrollbar = ttk.Scrollbar(self, orient="vertical", command=self.canvas.yview)
        self.canvas.configure(yscrollcommand=self.scrollbar.set)
        self.canvas.create_window((0, 0), window=self.scroll_frame, anchor="nw")
        self.scroll_frame.bind("<Configure>", lambda e: self.canvas.configure(scrollregion=self.canvas.bbox("all")))

        self.canvas.pack(side="left", fill="both", expand=True, padx=10, pady=5)
        self.scrollbar.pack(side="right", fill="y")

        # Bottom nav
        nav = tk.Frame(self, bg=BG_COLOR)
        nav.pack(pady=10)
        home_btn = ttk.Button(nav, text="🏠 Home", command=lambda: controller.show_frame(HomePage))
        stats_btn = ttk.Button(nav, text="📊 Stats", command=lambda: controller.show_frame(StatsPage))
        home_btn.grid(row=0, column=0, padx=5)
        stats_btn.grid(row=0, column=1, padx=5)
        ToolTip(home_btn, "Go Home")
        ToolTip(stats_btn, "View Stats")

    def add_task(self):
        text = self.task_entry.get().strip()
        if not text:
            messagebox.showwarning("Warning", "Please enter a task!")
            return
        # alternating colors
        color = CARD_COLOR if len(self.tasks) % 2 == 0 else "#f0f0f0"
        task_card = tk.Frame(self.scroll_frame, bg=color, bd=1, relief="solid")
        task_card.pack(fill="x", padx=8, pady=2)

        var = tk.BooleanVar()
        check = tk.Checkbutton(task_card, text=text, variable=var, bg=color,
                               font=("Segoe UI", 12), anchor="w",
                               command=lambda: self.toggle_done(check, var))
        check.pack(side="left", fill="x", expand=True, padx=10, pady=5)

        del_btn = ttk.Button(task_card, text="🗑️", width=3, command=lambda: self.delete_task(task_card, text, var))
        del_btn.pack(side="right", padx=8)
        ToolTip(del_btn, "Delete this task")

        self.tasks.append((task_card, var))
        self.task_entry.delete(0, tk.END)

    def toggle_done(self, check, var):
        if var.get():
            check.config(fg="gray", font=("Segoe UI", 12, "overstrike"))
        else:
            check.config(fg=TEXT_COLOR, font=("Segoe UI", 12))

    def delete_task(self, frame, text, var):
        frame.destroy()
        self.tasks = [t for t in self.tasks if t[0] != frame]
        # store deleted task for undo
        self.controller.deleted_tasks.append((text, var))

    def update_theme(self):
        # update background of all tasks
        for i, (frame, _) in enumerate(self.tasks):
            color = CARD_COLOR if i % 2 == 0 else "#f0f0f0"
            frame.config(bg=color)
            for widget in frame.winfo_children():
                if isinstance(widget, tk.Checkbutton):
                    widget.config(bg=color)


# --- STATISTICS PAGE ---
class StatsPage(tk.Frame):
    def __init__(self, parent, controller):
        super().__init__(parent, bg=BG_COLOR)
        self.controller = controller
        self.fig, self.ax = plt.subplots(figsize=(4, 3))
        self.canvas = FigureCanvasTkAgg(self.fig, master=self)
        self.canvas.get_tk_widget().pack(pady=20)

        nav = tk.Frame(self, bg=BG_COLOR)
        nav.pack(pady=10)
        ttk.Button(nav, text="Back Home", command=lambda: controller.show_frame(HomePage)).pack(pady=5)
        ttk.Button(nav, text="Refresh Stats", command=lambda: self.update_stats(controller)).pack(pady=5)

    def update_stats(self, controller):
        todo_page = controller.frames[ToDoPage]
        total = len(todo_page.tasks)
        done = sum(var.get() for _, var in todo_page.tasks)
        pending = total - done

        # bar chart
        self.ax.clear()
        self.ax.bar(["Completed", "Pending"], [done, pending], color=["green", "orange"])
        self.ax.set_ylim(0, max(total, 1))
        self.ax.set_title(f"Tasks (Total: {total})")
        self.canvas.draw()


# --- SETTINGS PAGE ---
class SettingsPage(tk.Frame):
    def __init__(self, parent, controller):
        super().__init__(parent, bg=BG_COLOR)
        tk.Label(self, text="Settings", bg=HEADER_COLOR,
                 fg="white", font=("Segoe UI", 18, "bold")).pack(fill="x", pady=10)

        ttk.Button(self, text="🌗 Toggle Theme", command=controller.toggle_theme).pack(pady=20)
        ttk.Button(self, text="Undo Deleted Task", command=self.undo_task).pack(pady=10)
        ttk.Button(self, text="Back Home", command=lambda: controller.show_frame(HomePage)).pack(pady=20)

    def undo_task(self):
        if self.master.controller.deleted_tasks:
            task = self.master.controller.deleted_tasks.pop()
            todo_page = self.master.controller.frames[ToDoPage]
            todo_page.tasks.append(task)
            # recreate frame
            todo_page.add_task()
            messagebox.showinfo("Undo", "Deleted task restored!")
        else:
            messagebox.showinfo("Undo", "No deleted tasks to restore.")

    def update_theme(self):
        pass


# --- ABOUT PAGE ---
class AboutPage(tk.Frame):
    def __init__(self, parent, controller):
        super().__init__(parent, bg=BG_COLOR)
        tk.Label(self, text="About", bg=HEADER_COLOR,
                 fg="white", font=("Segoe UI", 18, "bold")).pack(fill="x", pady=10)

        about_text = (
            "📘 About This App\n\n"
            "This To-Do List app helps you manage your daily tasks easily.\n"
            "✅ Add and mark tasks done\n"
            "📊 View stats of completed tasks\n"
            "⚙️ Customize theme in Settings\n\n"
            "Developed for the HCI Project using Tkinter."
        )
        tk.Label(self, text=about_text, bg=BG_COLOR, fg=TEXT_COLOR,
                 justify="left", font=("Segoe UI", 12), wraplength=400).pack(padx=30, pady=40)

        ttk.Button(self, text="Back Home", command=lambda: controller.show_frame(HomePage)).pack(pady=20)

    def update_theme(self):
        pass


# --- Run App ---
if __name__ == "__main__":
    app = ToDoApp()
    app.mainloop()
