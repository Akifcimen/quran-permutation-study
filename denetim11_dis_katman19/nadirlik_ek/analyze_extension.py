"""Recompute reported probabilities from exact counts and retained fixed-budget counts.

No simulation is silently extended by this analysis script. Scaled binomial
intervals apply only to independent exact-conditional draws, not SMC particles.
"""
from pathlib import Path
from fractions import Fraction
import sys, json, math, hashlib, os
ROOT=Path(__file__).resolve().parents[1]
if (ROOT/'.deps').exists(): sys.path.insert(0,str(ROOT/'.deps'))
import numpy as np
from scipy.stats import beta, t, norm
BASE=ROOT/'rare_event'
os.environ.setdefault('MPLCONFIGDIR',str(ROOT/'.cache/matplotlib'))
POLICIES=['uniform','mixture','literal','discussion','anchored19','frequency']
MODELS=['uniform','block6','inversion_exact']

def read(path): return json.loads((ROOT/path).read_text())
def cp(k,n):
    return [float(beta.ppf(.025,k,n-k+1)) if k else 0.,
            float(beta.ppf(.975,k+1,n-k)) if k<n else 1.]
def point(k,n,anchor=1.):
    p=anchor*k/n; ci=[anchor*x for x in cp(k,n)]
    return dict(hits=k,n=n,anchor_probability=anchor,p=p,cp95=ci,
                reciprocal=1/p if p else None,
                reciprocal_ci=[1/ci[1],1/ci[0]] if ci[0] else None)
def rep_summary(values):
    a=np.array(values,float); n=len(a); mean=float(a.mean())
    se=float(a.std(ddof=1)/math.sqrt(n)) if n>1 else None
    rse=se/mean if mean else None; share=float(a.max()/a.sum()) if a.sum() else None
    gates={'at_least_six_nonzero':bool(np.count_nonzero(a)>=6),
           'relative_se_at_most_035':rse is not None and rse<=.35,
           'maximum_share_at_most_035':share is not None and share<=.35}
    ci=[max(0.,mean-float(t.ppf(.975,n-1))*se),mean+float(t.ppf(.975,n-1))*se] if n>1 else None
    return dict(replicates=n,values=list(map(float,a)),mean=mean,se=se,relative_se=rse,
                maximum_replicate_share=share,nonzero=int(np.count_nonzero(a)),
                approximate_t_interval=ci,precision_gates=gates,
                passes_predeclared_precision_gates=all(gates.values()),
                caveat='Passing empirical gates does not prove all rare modes were sampled. The t interval is a replicate-based approximation, not an exact binomial interval.')

def analyze(require_production=True):
    c=read('rare_event/exact/block6_C_exact.json')
    cw=read('rare_event/exact/block6_CW_exact.json')
    exact_c=Fraction(int(c['reduced_numerator']),int(c['reduced_denominator']))
    exact_cw=Fraction(int(cw['reduced_numerator']),int(cw['reduced_denominator']))
    stored=read('rare_event/exact/conditional_results.json')
    result={'scope':'Extension of the frozen joint-grammar-v1 study. Fixed natural conjunctions, score-tail components and complete score-tail estimators are distinct estimands.',
            'conditional_independence':'No independence assumption between G,C,W,M. Exact conditioning plus IID draws.',
            'block6':{},'smc_production':{},'original_direct_test':{},'sources':{}}
    for name,ex in stored['experiments'].items():
        anchor=float(Fraction(int(ex['exact_anchor_numerator']),int(ex['exact_anchor_denominator'])))
        re={}
        for key,v in ex['estimates'].items():
            q=point(v['hits'],ex['n'],anchor)
            assert math.isclose(q['p'],v['unconditional_estimate'],rel_tol=1e-14,abs_tol=1e-300)
            assert np.allclose(q['cp95'],v['unconditional_CP95'],rtol=1e-12,atol=1e-300)
            re[key]=q
        result['block6'][name]={'n':ex['n'],'anchor':anchor,'estimates':re}
    result['block6']['chain']=[
        {'label':'C','p':float(exact_c),'cp95':None,'exact':True,'reciprocal':float(1/exact_c)},
        {'label':'C+W','p':float(exact_cw),'cp95':None,'exact':True,'reciprocal':float(1/exact_cw)},
        {'label':'C+W+G127',**result['block6']['C15_and_W']['estimates']['G127'],'exact':False},
        {'label':'C+W+G127+M0',**result['block6']['C15_and_W']['estimates']['G127_M_view0'],'exact':False}]
    result['block6']['conditional_factors']={'P_W_given_C':float(exact_cw/exact_c),
        'P_G127_given_CW':50888/100000000,'P_M0_given_CWG127':146/50888}
    result['block6']['same_sample_added_C_conditions']=stored['same_sample_added_C_conditions']
    old=read('results/summary.json')
    for model in MODELS:
        result['original_direct_test'][model]=old['models'][model]['policies']['mixture']
        rp=[BASE/'production'/f'{model}_rep{i}.json' for i in range(8)]
        complete=True; rows=[]
        for path in rp:
            try: row=json.loads(path.read_text())
            except (FileNotFoundError,json.JSONDecodeError): complete=False;break
            rows.append(row)
        if not complete:
            if require_production: raise RuntimeError('Production still running: '+model)
            result['smc_production'][model]={'status':'incomplete'};continue
        result['smc_production'][model]={'status':'complete',
            'primary':rep_summary([r['estimate']for r in rows]),
            'policies':{p:rep_summary([r['policy_estimates'][k]for r in rows])for k,p in enumerate(POLICIES)},
            'natural_events':{str(k):rep_summary([r['natural_event_estimates'][k]for r in rows])for k in range(4)}}
    b7=read('rare_event/block7/final_summary.json')
    br=point(b7['primary_hits'],b7['n'],float(Fraction(int(b7['B_exact']['numerator']),int(b7['B_exact']['denominator']))))
    assert math.isclose(br['p'],b7['p_component'],rel_tol=1e-14)
    assert np.allclose(br['cp95'],b7['CP95'],rtol=1e-12,atol=1e-300)
    global_upper=-math.expm1(math.log(.025)/1000000)
    assert math.isclose(global_upper,b7['full_block6_primary_tail_CI95'][1],rel_tol=1e-12)
    assert math.isclose(br['cp95'][0]/result['block6']['C15_and_W']['estimates']['G127_M_view0']['cp95'][1],b7['full_tail_to_natural19_ratio_one_sided_lower95'],rel_tol=1e-12)
    result['block7']=b7
    result['full_tail_bounds']={}
    for model in MODELS:
        if model=='block6':
            lower=br['cp95'][0]
            method='Exact conditional W7 component: one-sided 97.5% scaled Clopper–Pearson lower endpoint.'
        else:
            lower=.025*result['smc_production'][model]['primary']['mean']
            method='Markov inequality for the nonnegative unbiased mean of all eight fixed-budget SMC estimates: lower = .025 * mean.'
        result['full_tail_bounds'][model]={
            'p_interval_at_least_95_coverage':[lower,global_upper],
            'expected_waiting_interval':[1/global_upper,1/lower],
            'lower_bound_method':method,
            'upper_bound_method':'One-sided 97.5% exact binomial bound from the original zero exceedances in one million IID controls.',
            'coverage_scope':'Pointwise for this model and frozen test, by Bonferroni. Not simultaneous over all models or conditional on a precision-gate outcome.',
            'point_estimate_is_precise':False}
    uniform=read('rare_event/uniform_anchor/final_summary.json')
    batches=[read(f'rare_event/uniform_anchor/production_batch{i}.json')for i in range(2)]
    total_n=sum(b['n']for b in batches)
    assert total_n==uniform['n']==100000000
    by_batch=[{m['name']:m for m in b['metrics']}for b in batches]
    for name,reported in uniform['metrics'].items():
        if name not in by_batch[0]:continue
        total=sum(b[name]['weight_sum']for b in by_batch)
        square=sum(b[name]['weight_square_sum']for b in by_batch)
        hits=sum(b[name]['hits']for b in by_batch)
        p=total/total_n
        variance=max(0.,(square-total*total/total_n)/(total_n*(total_n-1)))
        se=math.sqrt(variance)
        interval=[max(0.,p-float(norm.ppf(.975))*se),p+float(norm.ppf(.975))*se]
        assert hits==reported['raw_hits']
        assert math.isclose(p,reported['estimate'],rel_tol=2e-14,abs_tol=1e-300)
        assert math.isclose(se,reported['SE'],rel_tol=2e-14,abs_tol=1e-300)
        assert np.allclose(interval,reported['normal_approx_95_interval'],rtol=2e-14,atol=1e-300)
    result['uniform_natural']={k:uniform[k]for k in [
        'scope','n','independent_worker_batches','all_proposals_in_denominator','pilots_excluded',
        'primary_event','primary','metrics','conditional_layer_steps','full_score_component_scope',
        'weight_formula','F_partial_matching_count_exact','precision_checks',
        'passes_predeclared_precision_checks','uncertainty','independent_anchor_comparison']}
    for path in ['protocol.json','rare_event/production/plan.json','rare_event/exact/production_plan.json','rare_event/exact/production_C10_plan.json','rare_event/exact/conditional_results.json','rare_event/block7/final_summary.json','rare_event/uniform_anchor/production_plan.json','rare_event/uniform_anchor/final_summary.json']:
        result['sources'][path]=hashlib.sha256((ROOT/path).read_bytes()).hexdigest()
    (BASE/'summary.json').write_text(json.dumps(result,indent=2,ensure_ascii=False)+'\n')
    return result

def figures(summary):
    import matplotlib
    matplotlib.use('Agg')
    import matplotlib.pyplot as plt
    from matplotlib.ticker import LogLocator,LogFormatterMathtext
    dest=BASE/'figures';dest.mkdir(exist_ok=True)
    plt.rcParams.update({'font.family':'DejaVu Sans','font.size':11,'axes.spines.top':False,'axes.spines.right':False,'axes.spines.left':False,'figure.facecolor':'white'})
    rows=summary['block6']['chain'];fig,ax=plt.subplots(figsize=(11,5.6))
    labels=['C: hücre ve katsayı düzeni','C + W: toplama–çıkarma seçimi','C + W + G: genel denge','C + W + G + M: doğal matris']
    for i,r in enumerate(rows):
        x=r['reciprocal'];y=3-i;color='#186782' if r['exact'] else '#a64b00'
        if r['cp95']:
            low,high=r['reciprocal_ci'];ax.errorbar(x,y,xerr=[[x-low],[high-x]],fmt='o',capsize=5,color=color,ms=8,lw=2)
        else: ax.plot(x,y,'D',color=color,ms=8)
        text={0:'1,13 milyon',1:'242 milyon',2:'475 milyar',3:'165,5 trilyon'}[i]
        ax.annotate(text,(x,y),xytext=(0,15),textcoords='offset points',ha='center',fontweight='bold',color=color)
    ax.set_xscale('log');ax.set_xlim(2e5,1e15);ax.set_ylim(-.45,3.6)
    ax.set_yticks(range(4));ax.set_yticklabels(labels[::-1]);ax.tick_params(axis='y',length=0,pad=12)
    ax.xaxis.set_major_locator(LogLocator(base=10,numticks=10));ax.xaxis.set_major_formatter(LogFormatterMathtext())
    ax.grid(axis='x',alpha=.18);ax.set_xlabel('Ortalama kaç bağımsız rastgele düzende bir?  (1 / p; logaritmik ölçek)',labelpad=12)
    fig.suptitle('Sabit 19 deseni: katmanlar eklendikçe sıklık',x=.02,ha='left',fontweight='bold',fontsize=16)
    fig.text(.02,.91,'Altılı blok modeli • 127 sayımı • aynı n + a / n − a kuralları • doğal matris sunumu',fontsize=10.5,color='#444')
    fig.text(.02,.02,'Elmas: tam sayım. Daire ve çizgi: 100 milyon tam koşullu örnekten %95 aralık.\nBu grafik, bütün alternatif kuralların tarandığı testin p-değerini göstermez.',fontsize=10,color='#444')
    fig.subplots_adjust(left=.35,right=.98,top=.83,bottom=.19)
    fig.savefig(dest/'sabit_19_katmanlari.png',dpi=170);plt.close(fig)
    s=summary['block6']['same_sample_added_C_conditions']
    names=['Eşit hücre sayıları','Kesintisiz destek ve sabit hücre','Her iki özellik birlikte']
    items=[s['equal_counts'],s['band_and_constant_cell'],s['both']]
    fig,ax=plt.subplots(figsize=(10,4.8))
    for y,v in enumerate(items[::-1]):
        p=v['estimate'];lo,hi=v['CP95'];ax.barh(y,p,color='#186782',height=.5)
        ax.errorbar(p,y,xerr=[[p-lo],[hi-p]],fmt='none',color='#1b263b',capsize=5)
        ax.text(.57,y,f'%{100*p:.1f}'.replace('.',','),va='center',fontweight='bold')
    ax.set_yticks(range(3));ax.set_yticklabels(names[::-1]);ax.tick_params(axis='y',length=0,pad=10)
    ax.set_xlim(0,.68);ax.set_xticks([0,.1,.2,.3,.4,.5]);ax.set_xticklabels(['%0','%10','%20','%30','%40','%50']);ax.grid(axis='x',alpha=.18)
    ax.set_xlabel('G, W, M ve merkez/toplam düzeni sağlanmışken ek özelliğin sıklığı')
    fig.suptitle('Ek hücre koşullarının birlikte katkısı',x=.02,ha='left',fontweight='bold',fontsize=16)
    fig.text(.02,.9,'Aynı örneklemde 189 temel başarı; bunların 36’sı iki ek koşulu da sağlıyor.',fontsize=11,color='#444')
    fig.text(.02,.02,'36 / 189 = %19,05 → birlikte yaklaşık 5,25 kat seyrekleşme. Çizgiler %95 Clopper–Pearson aralıkları.\nKoşulların ayrı olasılıkları birbiriyle çarpılmadı.',fontsize=10,color='#444')
    fig.subplots_adjust(left=.36,right=.96,top=.82,bottom=.23)
    fig.savefig(dest/'ek_hucre_kosullari.png',dpi=170);plt.close(fig)

if __name__=='__main__':
    import argparse
    ap=argparse.ArgumentParser();ap.add_argument('--allow-incomplete',action='store_true');ap.add_argument('--plots',action='store_true');a=ap.parse_args()
    result=analyze(not a.allow_incomplete)
    if a.plots:figures(result)
    print('Scaled intervals and conditional counts reconciled; summary saved.')
