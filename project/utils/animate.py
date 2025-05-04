import pandas as pd
import matplotlib.pyplot as plt
import time
import imageio.v2 as imageio
import os
import numpy as np
from io import BytesIO
from concurrent.futures import ProcessPoolExecutor

output_dir = os.path.abspath(os.path.join(os.path.dirname(__file__), "../build/output"))

# Настройки
x_count, y_count = 20, 20
w, h, z = 1, 1, 1
cx, cy, cz = 0.5, 0.5, -0.4
r = 0.4
u, v = np.mgrid[0:2*np.pi:20j, 0:np.pi:10j]
sphere_x = cx + r * np.cos(u) * np.sin(v)
sphere_y = cy + r * np.sin(u) * np.sin(v)
sphere_z = cz + r * np.cos(v)


def render_frame(fname_idx):
    fname, idx = fname_idx
    df = pd.read_csv(os.path.join(output_dir, fname))
    pts = df[['x', 'y', 'z']].values.reshape(y_count, x_count, 3)

    fig = plt.figure(figsize=(10, 10))
    ax = fig.add_subplot(111, projection='3d')
    ax.set_xlim([0, w])
    ax.set_ylim([0, h])
    ax.set_zlim([-z, 0])

    ax.scatter(df['x'], df['y'], df['z'], c='blue', s=3)

    for y in range(y_count):
        for x in range(x_count):
            p = pts[y][x]
            if x + 1 < x_count:
                q = pts[y][x + 1]
                ax.plot([p[0], q[0]], [p[1], q[1]], [p[2], q[2]], c='gray', linewidth=0.5)
            if y + 1 < y_count:
                q = pts[y + 1][x]
                ax.plot([p[0], q[0]], [p[1], q[1]], [p[2], q[2]], c='gray', linewidth=0.5)

    ax.plot_surface(sphere_x, sphere_y, sphere_z, color='orange', alpha=0.3, edgecolor='black')

    buf = BytesIO()
    plt.savefig(buf, format='png', dpi=80)
    plt.close(fig)
    buf.seek(0)
    img = imageio.imread(buf)
    return img

if __name__ == "__main__":
    start = time.perf_counter()

    files = sorted(
        [f for f in os.listdir(output_dir) if f.startswith("cloth_step_") and f.endswith(".csv")],
        key=lambda f: int(f.split("_")[-1].split(".")[0])
    )

    with ProcessPoolExecutor() as executor:
        images = list(executor.map(render_frame, [(f, i) for i, f in enumerate(files)]))

    writer = imageio.get_writer("cloth_parallel.mp4", fps=20)
    for img in images:
        writer.append_data(img)
    writer.close()

    end = time.perf_counter()
    print(f"✅ cloth_parallel.mp4 готов за {end - start:.2f} секунд")