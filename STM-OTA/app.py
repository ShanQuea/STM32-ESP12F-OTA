from flask import Flask, send_file, send_from_directory, render_template, request
from datetime import date
import os

app = Flask(__name__)

upload_folder = "uploaded_firmware"

def get_highest_version(upload_foloder):
    highest_version = 0

    for file in os.listdir(upload_folder):
        if file.startswith("firmware-") and file.endswith(".bin"):
            version_str = file.split("-")[1].split(".")[0]
            version = int(version_str)
            highest_version = max(highest_version, version)

    return highest_version


firmware_version = get_highest_version(upload_folder)


@app.route('/version', methods=['GET'])
def get_version():
    return str(firmware_version)


@app.route('/gethistory')
def version_history():
    files = os.listdir(upload_folder)
    history = [
        {'version': int(file.split('-')[1]), 'author': file.split('-')[2], 'date': file.split('-')[3].split('.')[0]}
        for file in files if file.startswith('firmware') and file.endswith('.bin')]
    return render_template('history.html', history=history)


@app.route('/download/<int:version>')
def download_firmware(version):
    files = [file for file in os.listdir(upload_folder) if
             file.startswith(f"firmware-{version}") and file.endswith(".bin")]
    if files:
        filename = files[0]
        filepath = os.path.join(upload_folder, filename)
        return send_file(filepath, as_attachment=True)
    else:
        return "该版本的固件文件不存在", 404


@app.route('/')
def index():
    return render_template('index.html')


@app.route('/update-firmware', methods=['POST'])
def update_firmware():
    global firmware_version
    firmware_version = int(request.form['version'])
    author = request.form.get('author')

    uploaded_file = request.files['firmware']
    today = date.today()
    today = today.strftime("%Y%m%d")
    if uploaded_file.filename != '':
        upload_path = os.path.join(upload_folder, f"firmware-{firmware_version}-{author}-{today}.bin")
        uploaded_file.save(upload_path)
        return "固件更新成功"
    else:
        return "没有文件选中", 400


if __name__ == '__main__':
    os.makedirs(upload_folder, exist_ok=True)
    app.run(debug=True, host='0.0.0.0', port=5000)
