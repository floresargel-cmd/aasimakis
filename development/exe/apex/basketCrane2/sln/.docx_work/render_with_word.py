import importlib.util, os
from pathlib import Path
base=Path.cwd()
script=Path('C:/Users/aflores/.codex/plugins/cache/openai-primary-runtime/documents/26.909.12148/skills/documents/render_docx.py')
spec=importlib.util.spec_from_file_location('renderer',script)
m=importlib.util.module_from_spec(spec);spec.loader.exec_module(m)
os.environ['PATH']='C:/Users/aflores/.cache/codex-runtimes/codex-primary-runtime/dependencies/native/poppler/Library/bin'+os.pathsep+os.environ['PATH']
m.convert_to_pdf=lambda *args,**kwargs:(str(base/'.docx_work/render/preview.pdf'),'Rendered with Microsoft Word because bundled LibreOffice is unavailable')
images=m.rasterize(str(base/'Apex_Basket_Crane_2_Current_Behavior.docx'),str(base/'.docx_work/render'),110,False,False)
print('\n'.join(images))
